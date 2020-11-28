@echo off

if exist "..\PBox\bin\Release\Pbox.exe" (
    set pbox_path="..\PBox\bin\Release\Pbox.exe"
) else if exist "..\PBox\bin\Debug\Pbox.exe" (
    set pbox_path="..\PBox\bin\Debug\Pbox.exe"
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

echo Found box at %pbox_path%
%pbox_path% --build data.box data.json
echo Done!