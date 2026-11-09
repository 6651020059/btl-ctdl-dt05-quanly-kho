#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

if ! command -v xelatex >/dev/null 2>&1; then
  echo "Khong tim thay xelatex. Hay cai TeX Live/MiKTeX va them xelatex vao PATH." >&2
  exit 2
fi

echo "[1/2] Bien dich bao cao bang XeLaTeX..."
xelatex -interaction=nonstopmode -halt-on-error -file-line-error main.tex
echo "[2/2] Cap nhat muc luc va tham chieu..."
xelatex -interaction=nonstopmode -halt-on-error -file-line-error main.tex
cp main.pdf report.pdf
echo "Hoan tat: $(pwd)/report.pdf"
