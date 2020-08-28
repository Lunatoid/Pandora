@echo off

if exist "..\bin\Release\PBox\Pbox.exe" (
    set pbox_path="..\bin\Release\PBox\Pbox.exe"
) else if exist "..\bin\Debug\PBox\Pbox.exe" (
    set pbox_path="..\bin\Debug\PBox\Pbox.exe"
) else (
    where /q "pbox.exe"
    if errorlevel 1 (
        where /q "box.exe"
        if errorlevel 1 (
            REM We have searched every location and we've not found it
            echo "Box not found. Has it been built?"
            exit
        ) else (
            set pbox_path="box.exe"
        )
    ) else (
        set pbox_path="pbox.exe"
    )
)

%pbox_path% --build data.box data.json
