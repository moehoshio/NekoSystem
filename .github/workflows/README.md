# NekoSystem GitHub Actions Workflows

This directory contains GitHub Actions workflows for the NekoSystem project.

## Workflows

### 1. CI (`ci.yml`)
**Trigger**: Push/PR to main/develop branches

**Purpose**: Continuous Integration testing across multiple platforms

**Jobs**:
- **Windows**: MSVC 2022 (Release/Debug), Clang
- **Linux**: GCC (Release/Debug), Clang  
- **macOS**: Clang (Release/Debug)

**Features**:
- Cross-platform compilation testing
- Automated unit test execution
- Build artifact upload on failure
- Parallel builds for faster CI

### 2. Code Quality (`code-quality.yml`)
**Trigger**: Push/PR to main/develop branches

**Purpose**: Code quality assurance and static analysis

**Jobs**:
- **Code formatting**: clang-format validation
- **Static analysis**: clang-tidy checks
- **Memory sanitizer**: AddressSanitizer testing
- **Thread sanitizer**: ThreadSanitizer testing  
- **Code coverage**: Coverage report generation

### 3. Release (`release.yml`)
**Trigger**: Git tags matching `v*`

**Purpose**: Automated release creation and asset building

**Jobs**:
- **Release creation**: GitHub release generation
- **Asset building**: Cross-platform binary packages
- **Documentation**: Doxygen docs deployment to GitHub Pages

**Artifacts**:
- `NekoSystem-Windows.zip`
- `NekoSystem-Linux.tar.gz`
- `NekoSystem-macOS.tar.gz`

### 4. Dependencies (`dependencies.yml`)
**Trigger**: Weekly schedule (Mondays 9 AM UTC) + manual

**Purpose**: Dependency management and security auditing

**Jobs**:
- **Dependency updates**: Automated dependency version updates
- **Security audit**: Security vulnerability scanning
- **PR creation**: Automated pull requests for updates

## Status Badges

Add these badges to your README.md:

```markdown
[![CI](https://github.com/moehoshio/NekoSystem/workflows/CI/badge.svg)](https://github.com/moehoshio/NekoSystem/actions/workflows/ci.yml)
[![Code Quality](https://github.com/moehoshio/NekoSystem/workflows/Code%20Quality/badge.svg)](https://github.com/moehoshio/NekoSystem/actions/workflows/code-quality.yml)
[![codecov](https://codecov.io/gh/moehoshio/NekoSystem/branch/main/graph/badge.svg)](https://codecov.io/gh/moehoshio/NekoSystem)
```

## Configuration

### Required Secrets
- `GITHUB_TOKEN`: Automatically provided by GitHub Actions

### Optional Setup
- **Codecov**: Sign up at [codecov.io](https://codecov.io) for coverage reports
- **GitHub Pages**: Enable in repository settings for documentation hosting

## Local Testing

To test workflows locally, you can use [act](https://github.com/nektos/act):

```bash
# Install act
# Windows (using winget)
winget install nektos.act

# Test CI workflow
act -j build-and-test

# Test specific job
act -j build-and-test -P ubuntu-latest=nektos/act-environments-ubuntu:18.04
```

## Workflow Files

| File | Purpose | Trigger |
|------|---------|---------|
| `ci.yml` | Cross-platform CI/CD | Push, PR |
| `code-quality.yml` | Static analysis, sanitizers | Push, PR |
| `release.yml` | Release automation | Git tags |
| `dependencies.yml` | Dependency management | Schedule, manual |

## Troubleshooting

### Common Issues

1. **Build failures on specific platforms**
   - Check compiler-specific warnings/errors
   - Verify C++20 feature compatibility
   - Review platform-specific code paths

2. **Test failures**
   - Check for platform-specific behavior differences
   - Verify test assumptions (file paths, permissions)
   - Review sanitizer reports for memory issues

3. **Dependency issues**
   - Verify FetchContent URLs are accessible
   - Check for version compatibility
   - Review CMake configuration

### Getting Help

- Check the [GitHub Actions documentation](https://docs.github.com/en/actions)
- Review workflow run logs in the Actions tab
- Open an issue with workflow-specific problems