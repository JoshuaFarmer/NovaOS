@echo off
bash make.sh
rem in my case, F: is a usb drive that i'll use as the medium to hold the OS.
set "ISOFilePath=%~dp0bin\Nova.iso"
echo The ISO file is located at: %ISOFilePath%
set "DestinationDir=F:/"

rem Create a virtual drive and mount the ISO
powershell.exe -NoProfile -Command "& {Mount-DiskImage -ImagePath '%ISOFilePath%' -PassThru | Get-Volume | ForEach-Object {if ($_.DriveType -eq 'CD-ROM') {$_.DriveLetter}}}" > temp.txt
set /p DriveLetter=<temp.txt
del /f temp.txt

rem Copy all files from the ISO to the destination directory
xcopy "%DriveLetter%:\*" "%DestinationDir%" /E /H /I

rem DismounAt the ISO
powershell.exe -NoProfile -Command "& {Dismount-DiskImage -ImagePath '%ISOFilePath%'}"
