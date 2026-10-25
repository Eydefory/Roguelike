#include "Ball.h"

#include "Constants.h"

#include <cmath>

namespace XYZRoguelike
{
    Ball::Ball()
        : speed(BALL_SPEED),
        startX(SCREEN_WIDTH / 2.f),
        startY(SCREEN_HEIGHT / 2.f)
    {
        shape.setRadius(BALL_RADIUS);
        shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
        shape.setFillColor(sf::Color::White);

        Reset();
    }

    void Ball::Reset()
    {
        speed = BALL_SPEED;
        shape.setPosition(startX, startY);

        velocity = sf::Vector2f(BALL_SPEED * 0.7f, -BALL_SPEED);

        float length = std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y
        );

        if (length > 0.f)
            velocity *= speed / length;
    }

    void Ball::Update(float deltaTime)
    {
        shape.move(velocity * deltaTime);

        sf::Vector2f position = shape.getPosition();

        if (position.x - BALL_RADIUS <= 0.f)
        {
            shape.setPosition(BALL_RADIUS, position.y);
            BounceHorizontal();
        }

        if (position.x + BALL_RADIUS >= SCREEN_WIDTH)
        {
            shape.setPosition(SCREEN_WIDTH - BALL_RADIUS, position.y);
            BounceHorizontal();
        }

        if (position.y - BALL_RADIUS <= 0.f)
        {
            shape.setPosition(position.x, BALL_RADIUS);
            BounceVertical();
        }
    }

    void Ball::Draw(sf::RenderWindow& window) const
    {
        window.draw(shape);
    }

    void Ball::BounceHorizontal()
    {
        velocity.x = -velocity.x;
    }

    void Ball::BounceVertical()
    {
        velocity.y = -velocity.y;
    }

    bool Ball::CheckPlatformCollision(const sf::FloatRect& platformBounds)
    {
        sf::FloatRect ballBounds = GetBounds();

        if (!ballBounds.intersects(platformBounds))
            return false;

        if (velocity.y <= 0.f)
            return false;

        shape.setPosition(
            shape.getPosition().x,
            platformBounds.top - BALL_RADIUS
        );

        BounceVertical();

        float platformCenter =
            platformBounds.left + platformBounds.width / 2.f;

        float ballCenter = shape.getPosition().x;

        float difference = ballCenter - platformCenter;

        float normalizedDifference =
            difference / (platformBounds.width / 2.f);

        velocity.x = normalizedDifference * speed;

        float currentSpeed = std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y
        );

        if (currentSpeed > 0.f)
            velocity *= speed / currentSpeed;

        return true;
    }

    bool Ball::IsOutOfScreen() const
    {
        return shape.getPosition().y - BALL_RADIUS > SCREEN_HEIGHT;
    }

    sf::Vector2f Ball::GetPosition() const
    {
        return shape.getPosition();
    }

    sf::FloatRect Ball::GetBounds() const
    {
        return shape.getGlobalBounds();
    }

    void Ball::SetPosition(const sf::Vector2f& position)
    {
        shape.setPosition(position);
    }

    void Ball::SetSpeedMultiplier(float multiplier)
    {
        float length = std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y
        );

        speed = BALL_SPEED * multiplier;

        if (length > 0.f)
            velocity *= speed / length;
    }

    void Ball::ResetSpeed()
    {
        float length = std::sqrt(
            velocity.x * velocity.x +
            velocity.y * velocity.y
        );

        speed = BALL_SPEED;

        if (length > 0.f)
            velocity *= speed / length;
    }
}