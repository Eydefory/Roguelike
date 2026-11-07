
#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

enum class LogLevel
{
    Info,
    Warning,
    Error
};

class LogSink
{
public:
    virtual void log(LogLevel level, const std::string& message) = 0;
    virtual ~LogSink() = default;
};

inline std::string LogLevelToString(LogLevel level)
{
    switch (level)
    {
    case LogLevel::Info:
        return "[INFO]";
    case LogLevel::Warning:
        return "[WARNING]";
    case LogLevel::Error:
        return "[ERROR]";
    default:
        return "[UNKNOWN]";
    }
}

class ConsoleSink : public LogSink
{
public:
    void log(LogLevel level, const std::string& message) override
    {
        std::cout << LogLevelToString(level) << " " << message << '\n';
        if (level == LogLevel::Error)
        {
            std::cout.flush();
        }
    }
};

class FileSink : public LogSink
{
public:
    explicit FileSink(const std::string& filename)
    {
        logFile.open(filename, std::ios::app);
    }

    void log(LogLevel level, const std::string& message) override
    {
        if (!logFile.is_open())
            return;

        logFile << LogLevelToString(level) << " " << message << '\n';
        if (level == LogLevel::Error)
        {
            logFile.flush();
        }
    }

    bool IsOpen() const
    {
        return logFile.is_open();
    }

private:
    std::ofstream logFile;
};

class Logger
{
public:
    void addSink(std::shared_ptr<LogSink> sink)
    {
        if (!sink)
            return;

        std::lock_guard<std::mutex> lock(logMutex);
        sinks.push_back(sink);
    }

    void log(LogLevel level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        for (auto& sink : sinks)
        {
            try
            {
                sink->log(level, message);
            }
            catch (...)
            {
                std::cerr << "[ERROR] Logger sink failed" << std::endl;
            }
        }
    }

    void info(const std::string& message)
    {
        log(LogLevel::Info, message);
    }

    void warn(const std::string& message)
    {
        log(LogLevel::Warning, message);
    }

    void error(const std::string& message)
    {
        log(LogLevel::Error, message);
    }

private:
    std::vector<std::shared_ptr<LogSink>> sinks;
    std::mutex logMutex;
};

class LoggerRegistry
{
public:
    static LoggerRegistry& getInstance()
    {
        static LoggerRegistry instance;
        return instance;
    }

    std::shared_ptr<Logger> getLogger(const std::string& name)
    {
        std::lock_guard<std::mutex> lock(registryMutex);

        auto it = loggers.find(name);

        if (it != loggers.end())
            return it->second;

        return defaultLogger;
    }

    void setDefaultLogger(std::shared_ptr<Logger> logger)
    {
        if (!logger)
        {
            return;
        }

        std::lock_guard<std::mutex> lock(registryMutex);
        defaultLogger = logger;
    }

    void registerLogger(const std::string& name, std::shared_ptr<Logger> logger)
    {
        if (name.empty() || !logger)
            return;

        std::lock_guard<std::mutex> lock(registryMutex);
        loggers[name] = logger;
    }

private:
    LoggerRegistry()
    {
        defaultLogger = std::make_shared<Logger>();
        defaultLogger->addSink(std::make_shared<ConsoleSink>());
    }

    std::unordered_map<std::string, std::shared_ptr<Logger>> loggers;
    std::shared_ptr<Logger> defaultLogger;
    std::mutex registryMutex;
};

#define LOG_INFO(message) LoggerRegistry::getInstance().getLogger("global")->info(message)
#define LOG_WARN(message) LoggerRegistry::getInstance().getLogger("global")->warn(message)
#define LOG_ERROR(message) LoggerRegistry::getInstance().getLogger("global")->error(message)
