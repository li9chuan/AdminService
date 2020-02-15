@echo off

REM This script will kill all the services launched by _shard_start.bat

rem  ns
taskkill /IM naming_service.exe /F
taskkill /IM frontend_service.exe /F
taskkill /IM player_logic_service.exe /F
taskkill /IM schedule_service.exe /F
taskkill /IM persistant_data_service.exe /F
