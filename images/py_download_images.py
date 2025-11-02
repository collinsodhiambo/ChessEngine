#!/usr/bin/env python3
"""
Download every file in public/piece/cburnett at a specific commit using the GitHub API.
Usage:
  GITHUB_TOKEN=ghp_xxx python3 download_images.py
or
  python3 download_images.py
Requires: Python 3.6+
"""

import os
import sys
import json
from urllib.request import Request, urlopen
from urllib.error import HTTPError, URLError
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

OWNER = "lichess-org"
REPO = "lila"
REF = "95040a43332c72d9d82a8afd21e48ce1a7584360"
PATH_ON_REPO = "public/piece/cburnett"
API_URL = f"https://api.github.com/repos/{OWNER}/{REPO}/contents/{PATH_ON_REPO}?ref={REF}"

token = os.environ.get("GITHUB_TOKEN")

headers = {"User-Agent": "download-images-script"}
if token:
    headers["Authorization"] = f"token {token}"

def get_json(url):
    req = Request(url, headers=headers)
    with urlopen(req) as resp:
        return json.load(resp)

def download(url):
    try:
        name = Path(url).name
        print(f"Downloading {name}")
        req = Request(url, headers=headers)
        with urlopen(req) as r, open(name, "wb") as f:
            f.write(r.read())
    except (HTTPError, URLError) as e:
        print(f"Failed to download {url}: {e}", file=sys.stderr)

def main():
    try:
        data = get_json(API_URL)
    except Exception as e:
        print(f"Failed to fetch file list: {e}", file=sys.stderr)
        sys.exit(1)

    download_urls = [item.get("download_url") for item in data if item.get("download_url")]
    if not download_urls:
        print("No files found.", file=sys.stderr)
        sys.exit(1)

    # Download concurrently
    with ThreadPoolExecutor(max_workers=6) as ex:
        for url in download_urls:
            ex.submit(download, url)

if __name__ == "__main__":
    main()