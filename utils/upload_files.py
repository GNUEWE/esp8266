#!/usr/bin/env python3
"""
Upload Files to ESP8266
========================
Utility script to upload multiple files to ESP8266 via serial connection.

Requirements:
    pip install adafruit-ampy

Usage:
    python upload_files.py <port> <file1> [file2] [file3] ...

Examples:
    python upload_files.py /dev/ttyUSB0 main.py boot.py
    python upload_files.py COM3 ssd1306.py bme280.py
"""

import sys
import os
import subprocess
import time

def upload_file(port, filename):
    """Upload a single file to ESP8266"""
    if not os.path.exists(filename):
        print(f"❌ File not found: {filename}")
        return False
    
    print(f"📤 Uploading {filename}...", end=' ')
    try:
        # Use ampy to upload file
        result = subprocess.run(
            ['ampy', '--port', port, 'put', filename],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.returncode == 0:
            print("✓")
            return True
        else:
            print(f"✗\n   Error: {result.stderr}")
            return False
    except subprocess.TimeoutExpired:
        print("✗ (timeout)")
        return False
    except FileNotFoundError:
        print("✗\n   Error: ampy not found. Install with: pip install adafruit-ampy")
        return False
    except Exception as e:
        print(f"✗\n   Error: {e}")
        return False

def list_files(port):
    """List files on ESP8266"""
    try:
        result = subprocess.run(
            ['ampy', '--port', port, 'ls'],
            capture_output=True,
            text=True,
            timeout=10
        )
        if result.returncode == 0:
            return result.stdout.strip().split('\n')
        return []
    except:
        return []

def main():
    if len(sys.argv) < 3:
        print("Usage: python upload_files.py <port> <file1> [file2] [file3] ...")
        print("\nExamples:")
        print("  python upload_files.py /dev/ttyUSB0 main.py boot.py")
        print("  python upload_files.py COM3 ssd1306.py bme280.py")
        sys.exit(1)
    
    port = sys.argv[1]
    files = sys.argv[2:]
    
    print(f"🔌 Connecting to ESP8266 on {port}")
    print(f"📁 Files to upload: {len(files)}")
    print("-" * 50)
    
    # Upload each file
    success_count = 0
    failed_count = 0
    
    for filename in files:
        if upload_file(port, filename):
            success_count += 1
        else:
            failed_count += 1
        time.sleep(0.5)  # Small delay between uploads
    
    print("-" * 50)
    print(f"✓ Uploaded: {success_count}")
    print(f"✗ Failed: {failed_count}")
    
    # List files on device
    print("\n📋 Files on ESP8266:")
    device_files = list_files(port)
    if device_files:
        for f in device_files:
            print(f"  - {f}")
    else:
        print("  (could not list files)")
    
    print("\n✅ Done!")

if __name__ == '__main__':
    main()
