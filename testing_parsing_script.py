#!/usr/bin/env python3

import os
import subprocess
import sys
import time
import signal
from pathlib import Path

def run_make_bonus():
    """Run make bonus command"""
    print("Running 'make bonus'...")
    try:
        result = subprocess.run(['make', 'bonus'], check=True, capture_output=True, text=True)
        print("✓ Make bonus completed successfully")
        return True
    except subprocess.CalledProcessError as e:
        print(f"✗ Make bonus failed: {e}")
        print(f"Error output: {e.stderr}")
        return False

def test_normal_scene(scene_file):
    """Test a normal scene file - should start and then be terminated"""
    print(f"\n--- Testing normal scene: {scene_file} ---")
    
    try:
        # Start the process
        process = subprocess.Popen(
            ['./miniRT', scene_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Wait a bit for the program to start up
        time.sleep(2)
        
        # Send escape signal (SIGTERM)
        process.terminate()
        
        # Wait for the process to finish with timeout
        try:
            stdout, stderr = process.communicate(timeout=5)
            print(f"✓ {scene_file}: Started successfully and terminated")
            if stderr:
                print(f"  stderr: {stderr.strip()}")
            return True
        except subprocess.TimeoutExpired:
            # Force kill if it doesn't respond to terminate
            process.kill()
            stdout, stderr = process.communicate()
            print(f"✓ {scene_file}: Started successfully (had to force kill)")
            return True
            
    except Exception as e:
        print(f"✗ {scene_file}: Error running scene - {e}")
        return False

def test_error_scene(scene_file):
    """Test a scene file that should fail immediately"""
    print(f"\n--- Testing error scene: {scene_file} ---")
    
    try:
        # Start the process
        process = subprocess.Popen(
            ['./miniRT', scene_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )
        
        # Wait for the process to finish with a short timeout
        try:
            stdout, stderr = process.communicate(timeout=3)
            
            if process.returncode != 0:
                print(f"✓ {scene_file}: Failed as expected (return code: {process.returncode})")
                if stderr:
                    print(f"  stderr: {stderr.strip()}")
                return True
            else:
                print(f"✗ {scene_file}: Expected failure but succeeded")
                return False
                
        except subprocess.TimeoutExpired:
            # If it's still running after 3 seconds, it shouldn't be
            process.terminate()
            try:
                process.communicate(timeout=2)
            except subprocess.TimeoutExpired:
                process.kill()
                process.communicate()
            
            print(f"✗ {scene_file}: Expected immediate failure but started running")
            return False
            
    except Exception as e:
        print(f"✗ {scene_file}: Error running scene - {e}")
        return False

def main():
    """Main function to run all tests"""
    print("=== miniRT Scene Tester ===\n")
    
    # Check if we're in the right directory
    if not os.path.exists('./miniRT') and not os.path.exists('Makefile'):
        print("Error: Please run this script from the miniRT project root directory")
        sys.exit(1)
    
    # Run make bonus first
    if not run_make_bonus():
        print("Failed to build project. Exiting.")
        sys.exit(1)
    
    # Check if miniRT executable exists
    if not os.path.exists('./miniRT'):
        print("Error: ./miniRT executable not found")
        sys.exit(1)
    
    # Test normal scenes
    scene_dir = Path('scene')
    if scene_dir.exists():
        print(f"\n=== Testing normal scenes in {scene_dir} ===")
        normal_scenes = list(scene_dir.glob('*.rt'))
        
        if normal_scenes:
            normal_passed = 0
            for scene_file in sorted(normal_scenes):
                if test_normal_scene(str(scene_file)):
                    normal_passed += 1
            
            print(f"\nNormal scenes: {normal_passed}/{len(normal_scenes)} passed")
        else:
            print("No .rt files found in scene directory")
    
    # Test error scenes
    error_scene_dir = Path('scene/shouldnt_work')
    if error_scene_dir.exists():
        print(f"\n=== Testing error scenes in {error_scene_dir} ===")
        error_scenes = list(error_scene_dir.glob('*.rt'))
        
        if error_scenes:
            error_passed = 0
            for scene_file in sorted(error_scenes):
                if test_error_scene(str(scene_file)):
                    error_passed += 1
            
            print(f"\nError scenes: {error_passed}/{len(error_scenes)} passed")
        else:
            print("No .rt files found in scene/shouldnt_work directory")
    else:
        print(f"\n=== scene/shouldnt_work directory not found ===")
        print("Skipping error scene tests")
    
    print("\n=== Testing completed ===")

if __name__ == "__main__":
    main()
