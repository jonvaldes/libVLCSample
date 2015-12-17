# libVLCSample
Sample showing how to easily use libVLC to play videos

This sample loads the dll and gets each function directly by using GetProcAddress, so 90% of the code is just 
dealing with that and the Windows API to get a window up and handle its events. The actual VLC integration is 
just the last 15 lines in video.cpp.
