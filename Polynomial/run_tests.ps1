$ErrorActionPreference = "Stop"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "      Polynomial Roots Finding Tests      " -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan

# Define build directory (relative to the script location)
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Definition
$BuildDir = Join-Path $ScriptDir "build"

# Check if build directory exists
if (-not (Test-Path -Path $BuildDir)) {
    Write-Host "Build directory not found. Creating..." -ForegroundColor Yellow
    New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
}

Set-Location -Path $BuildDir

# 1. Configure CMake (optional, but ensures generated files are up to date)
Write-Host "`n[1/3] Configuring CMake..." -ForegroundColor Green
try {
    cmake ..
    if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed." }
}
catch {
    Write-Host "Error during CMake configuration: $_" -ForegroundColor Red
    exit 1
}

# 2. Build the project
Write-Host "`n[2/3] Building Project..." -ForegroundColor Green
try {
    cmake --build . --config Release
    if ($LASTEXITCODE -ne 0) { throw "Build failed." }
}
catch {
    Write-Host "Error during Build: $_" -ForegroundColor Red
    exit 1
}

# 3. Run Tests
Write-Host "`n[3/3] Running Tests..." -ForegroundColor Green
try {
    # Run ctest with output on failure and verbose output
    # -C Release: Specify configuration for multi-config generators
    # --output-on-failure: Show log only if test fails
    ctest -C Release --output-on-failure --verbose
    
    if ($LASTEXITCODE -eq 0) {
        Write-Host "`n[SUCCESS] All tests passed!" -ForegroundColor Green
    }
    else {
        Write-Host "`n[FAILURE] Some tests failed." -ForegroundColor Red
    }
}
catch {
    Write-Host "Error during Test Execution: $_" -ForegroundColor Red
    exit 1
}
