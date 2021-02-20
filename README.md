[![Issues][issues-shield]][issues-url]
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]


<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/vvvar/PeakEater">
    <img src="Resources/logo_full.png" alt="Logo">
  </a>

  <h3 align="center">PeakEater</h3>

  <p align="center">
    Free open source VST3/AU wave shaping plugin.
    <br />
    <!-- <a href=""><strong>Explore the docs »</strong></a> -->
    <!-- <br />
    <br /> -->
    <!-- <a href="">View Demo</a>
    ·
    <a href="">Report Bug</a>
    ·
    <a href="">Request Feature</a> -->
  </p>
</p>



<!-- TABLE OF CONTENTS -->
<details open="open">
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#features">Features</a></li>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgements">Acknowledgements</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://github.com/vvvar/PeakEater)

PeakEater is a free, easy to use, open-source waveshaping VST3/AU plugin that lets you choose between different waveshaping [functions](https://en.wikipedia.org/wiki/Sigmoid_function) to boost the overall volume level of your track. It supports variours <a href="#features">features</a> such as multiple clipping types, oversampling and visualization of clipping process that aims to make clipping easy.

Inspiration for this plugin comes mostly from [KAZROG KClip 3](https://kazrog.com/products/kclip-3) and [VennAudio Free Clip](https://www.vennaudio.com/free-clip/). So it's an attempt to combine the features of both plugins but completley free and open-source.

### Features

##### Input/Output RMS meters
See the RMS of signal before and after plugin.

##### Input/Output Gain
Adjusts volume before or after signal processing.

##### Link Input Gain with Output Gain
Use linking to automatically set the Output Gain level to the opposite of the Input Gain. 
As an example, if the Input Gain value is +5dB then the Output Gain value will be automatically set to -5dB. It also works in the opposite way.

##### Ceiling control
See the level of signal after clipping using the Ceiling slider and adjust it to control at which dB level clipping function will start to limit the signal.

##### 6 Clipping yypes to choose
Choose between 6 clipping functions, from harshest "Hard" to softest "Arctangent".

##### Up to 16x Oversampling
Choose between 2x, 4x, 8x, and 16x oversampling to avoid signal aliasing. Be aware that high values of oversampling may hurt CPU performance.

##### See how much dBs were clipped
EATEN provides information about how much dB's were cut by waveshaper so you can see how much volume you're won. 

##### Bypass mode
Simply bypass all plugin's processing.

##### In/Out meters
See levels of the signal before and after the plugin's processing.

##### VST3 and AU support
You can use either VST3 or AU type of plugin on macOS and VST3 on Windows.

### Built With

This plugin was developed using [JUCE](https://github.com/juce-framework/JUCE) framework and [ff_meters](https://github.com/ffAudio/ff_meters) library that provided convinient dB-meters.


<!-- GETTING STARTED -->
## Getting Started

If you need a ready-to-use build, you may download prebuild packages from the [Releases Page](https://github.com/vvvar/PeakEater/releases).
The guideline provided below is needed if you would like to build a plugin on your own.

### Prerequisites

- [Git](https://git-scm.com) v2.24.3+
- [Projucer](https://juce.com/discover/projucer) v6.0.7+
- XCode v10.2+

### Installation

Once you have the dependencies installed, we need to clone the PeakEater repository. PeakEater's git repository contains necessary submodules, so we'll need to collect those as well, which we can do one of two ways:
```
git clone --recurse-submodules https://github.com/vvvar/PeakEater.git
```
or:
```
git clone https://github.com/vvvar/PeakEater.git
cd PeakEater
git submodule update --init --recursive
```

At this point, you should have a cloned project with all necessary dependencies installed into the [```Dependencies```](https://github.com/vvvar/PeakEater/tree/master/Dependencies) directory.

Now, open [```PeakEater.jucer```](https://github.com/vvvar/PeakEater/blob/master/PeakEater.jucer) file using <a href="#prerequisites">Projucer</a> and press Save and Open in IDE.

That's it, you may now build PeakEater for the available targets.

Please note that in order to disable JUCE Splash Screen you should switch your Projucer into GPL3 mode. If you're opening Projucer for the first time, popup regarding login should appear in the bottom left side of Projucer. Click "Singn In" and then choose "Use GPL3 Mode" instead of making sign in.

<!-- ROADMAP -->
## Roadmap

Check out [Project Roadmap](https://github.com/vvvar/PeakEater/projects/1) page for more details.

<!-- CONTRIBUTING -->
## Contributing

This is a very young project on a very early stages, however, I will be happy to hear any sort of a feedback on it.
If you have any feedback on a project, feel free to open a issue using [Issues Page](https://github.com/vvvar/PeakEater/issues).
If you have a pull request, then just make it using common [Fork & PR flow](https://docs.github.com/en/github/collaborating-with-issues-and-pull-requests/creating-a-pull-request-from-a-fork).

<!-- LICENSE -->
## License

Distributed under the GPL-3.0 License. See [`LICENSE.md`](https://github.com/vvvar/PeakEater/blob/master/LICENSE.md) for more information.


<!-- CONTACT -->
## Contact

Vladyslav Voinov - [Linkedin](https://www.linkedin.com/in/vladyslav-voinov-5126a793/) - voinovvladv@gmail.com

Project Link: [https://github.com/vvvar/PeakEater](https://github.com/vvvar/PeakEater)



<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
* [JUCE](https://github.com/juce-framework/JUCE)
* [ff_meters](https://github.com/ffAudio/ff_meters) by [ffAudio](https://github.com/ffAudio)
* [Free Clip](https://gitlab.com/JHVenn/Free-Clip) by [Venn Audio](https://www.vennaudio.com)
* [The Audio Programmer](https://theaudioprogrammer.com) for providing extensive video-lessons on audio programming and DSP



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[issues-shield]: https://img.shields.io/github/issues/vvvar/PeakEater?style=for-the-badge
[issues-url]: https://github.com/vvvar/PeakEater/issues
[license-shield]: https://img.shields.io/github/license/vvvar/PeakEater?style=for-the-badge
[license-url]: https://github.com/vvvar/PeakEater/blob/master/LICENSE.md
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/vladyslav-voinov-5126a793/
[product-screenshot]: Resources/screenshot.png
