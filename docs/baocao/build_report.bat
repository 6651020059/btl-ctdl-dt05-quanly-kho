@echo off
setlocal
cd /d "%~dp0"
where xelatex >nul 2>nul
if errorlevel 1 goto noxelatex

echo [1/3] Dang bien dich LaTeX bang XeLaTeX...
xelatex -interaction=nonstopmode -halt-on-error -file-line-error main.tex
if errorlevel 1 goto error

echo [2/3] Cap nhat muc luc va tham chieu...
xelatex -interaction=nonstopmode -halt-on-error -file-line-error main.tex
if errorlevel 1 goto error

echo [3/3] Chot so trang, muc luc va tham chieu...
xelatex -interaction=nonstopmode -halt-on-error -file-line-error main.tex
if errorlevel 1 goto error

if exist main.pdf copy /Y main.pdf report.pdf >nul
echo.
echo Hoan tat. File PDF: report.pdf
exit /b 0

:noxelatex
echo Khong tim thay xelatex trong PATH.
echo Hay cai MiKTeX hoac TeX Live, sau do mo lai cua so nay.
exit /b 2

:error
echo.
echo Co loi bien dich. Hay mo main.log va tim dong bat dau bang dau cham than (!).
exit /b 1
