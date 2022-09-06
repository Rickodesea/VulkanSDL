# VulkanSDL
Vulkan SDL Template

This is a template code base for implement Vulkan on SDL.  It was coded on linux but should
work on other platforms.  The code is meant for quick re-use (as is or freely edited).
It is written completely in C (hardly any C Vulkan tutorial out there) and the code is well documented.

## License
This project is licensed under the zlib license - see the [LICENSE](LICENSE) file for details.
© All rights reserved.

## Version
The current version of this project can be seen on the [VERSION](VERSION.md) file.

## Project Managers
* Innovative Director:		Alrick Grandison


## Authors
See [AUTHORS](AUTHORS) for full list.

## Contributing
See [CONTRIBUTING.md](CONTRIBUTING.md) for how to contribute.


<br/><br/>

If you are getting this warning when you run the code on Linux:```
```
MESA-INTEL: warning: Performance support disabled, consider sysctl dev.i915.perf_stream_paranoid=0
```

Try running this to resolve:
```
sudo sysctl -w dev.i915.perf_stream_paranoid=0
```

