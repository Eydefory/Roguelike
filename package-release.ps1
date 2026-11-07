$ErrorActionPreference = 'Stop'

$projectRoot = $PSScriptRoot
$buildDirectory = Join-Path $projectRoot 'SFMLTemplate\x64\Release'
$releaseDirectory = Join-Path $projectRoot 'release'
$archivePath = Join-Path $releaseDirectory 'XYZ-Roguelike.zip'
$resourceDirectory = Join-Path $projectRoot 'SFMLTemplate\Resources'
$runtimeFiles = @(
    'SFMLTemplate.exe',
    'Engine.dll',
    'openal32.dll',
    'sfml-audio-2.dll',
    'sfml-graphics-2.dll',
    'sfml-network-2.dll',
    'sfml-system-2.dll',
    'sfml-window-2.dll'
)

foreach ($fileName in $runtimeFiles) {
    $filePath = Join-Path $buildDirectory $fileName
    if (-not (Test-Path -LiteralPath $filePath -PathType Leaf)) {
        throw "Release file is missing: $filePath"
    }
}

if (-not (Test-Path -LiteralPath $resourceDirectory -PathType Container)) {
    throw "Resource directory is missing: $resourceDirectory"
}

$stagePrefix = 'XYZ-Roguelike-release-'
$tempRoot = [System.IO.Path]::GetFullPath([System.IO.Path]::GetTempPath())
$stageDirectory = Join-Path $tempRoot ($stagePrefix + [guid]::NewGuid().ToString('N'))
$stageFullPath = [System.IO.Path]::GetFullPath($stageDirectory)
$tempRootWithSeparator = $tempRoot.TrimEnd('\') + '\'

if (-not $stageFullPath.StartsWith($tempRootWithSeparator, [System.StringComparison]::OrdinalIgnoreCase)) {
    throw "Unexpected staging path: $stageFullPath"
}

New-Item -ItemType Directory -Path $releaseDirectory -Force | Out-Null
New-Item -ItemType Directory -Path $stageFullPath | Out-Null

try {
    foreach ($fileName in $runtimeFiles) {
        Copy-Item -LiteralPath (Join-Path $buildDirectory $fileName) -Destination $stageFullPath
    }

    Copy-Item -LiteralPath $resourceDirectory -Destination $stageFullPath -Recurse
    Compress-Archive -Path (Join-Path $stageFullPath '*') -DestinationPath $archivePath -CompressionLevel Optimal -Force

    Write-Output "Release archive created: $archivePath"
}
finally {
    $resolvedStage = (Resolve-Path -LiteralPath $stageFullPath).Path
    if ($resolvedStage.StartsWith($tempRootWithSeparator, [System.StringComparison]::OrdinalIgnoreCase) -and
        (Split-Path -Leaf $resolvedStage).StartsWith($stagePrefix, [System.StringComparison]::Ordinal)) {
        Remove-Item -LiteralPath $resolvedStage -Recurse -Force
    }
}
