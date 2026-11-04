"""Capture the real QML interface and assemble the README animation."""

import argparse
from pathlib import Path
import subprocess
import tempfile

try:
    from PIL import Image
except ImportError:
    raise SystemExit("GIF generation requires Pillow: python3 -m pip install Pillow")

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("capture_tool")
parser.add_argument("output", type=Path)
args = parser.parse_args()

with tempfile.TemporaryDirectory(prefix="qtsudoku-frames-") as directory:
    subprocess.run([args.capture_tool, directory], check=True, timeout=90)
    frames = []
    for path in sorted(Path(directory).glob("*.png")):
        with Image.open(path) as image:
            frames.append(image.convert("RGB").quantize(colors=128))
    if len(frames) < 2:
        raise SystemExit("Capture tool did not produce an animation")
    args.output.parent.mkdir(parents=True, exist_ok=True)
    temporary = args.output.with_suffix(".tmp.gif")
    frames[0].save(
        temporary, save_all=True, append_images=frames[1:],
        duration=100, loop=0, optimize=True, disposal=1,
    )
    temporary.replace(args.output)
    print(f"Updated {args.output} ({len(frames)} frames)")
