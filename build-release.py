#!/usr/bin/env python3
import shutil
import subprocess
import argparse
import subprocess
from pathlib import Path

FRONTEND_IMAGE_NAME = "planttracker-frontend"
FRONTEND_PATH_INSIDE_CONTAINER = "/app/dist"

def execute_command(command: list[str]) -> None:
    subprocess.run(command, check=True)

def build_frontend(output_path: Path) -> None:
    """Build the app using the appropriate command."""
    execute_command(["docker", "build", "-t", FRONTEND_IMAGE_NAME, "frontend"])
    # Get the container ID of the temporary container
    result = subprocess.run(["docker", "create", FRONTEND_IMAGE_NAME], check=True, text=True, capture_output=True)
    container_id = result.stdout.strip()
    execute_command(["docker", "cp", f'{container_id}:{FRONTEND_PATH_INSIDE_CONTAINER}', str(output_path)])
    execute_command(["docker", "rm", "-v", container_id])

def build_backend(output_path: Path) -> None:
    """Build the app using the appropriate command."""
    execute_command(["cargo", "build", "--release"])
    execute_command(["cp", "target/release/planttracker-backend", str(output_path)])

def main() -> None:
    """Entry point of the script."""
    # Parse command line arguments
    parser = argparse.ArgumentParser()
    parser.add_argument("--output-path", help="Destination path to copy the app", type=Path)
    args = parser.parse_args()
    output_path = Path(args.output_path).resolve()
    if output_path.exists():
        shutil.rmtree(output_path)

    output_path.mkdir(parents=True)

    # Build the frontend
    build_frontend(output_path / "frontend")

    # Build the backend
    build_backend(output_path)


if __name__ == "__main__":
    main()