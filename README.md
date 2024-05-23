# Simulator
**An animation that shows an oil-stained area with bacteria that clean it up.**

An oil-stained area represented by black pixels scattered within a rectangle.
Bacteria of two kinds represented by colored squares move among the oil particles and "eat" the particle on meeting it.

"Hunter" - bacteria that can sense the particles and move directly towards them. (Red squares)
"Runner" - bacteria that can't sense but moves on randomal directions and eat the particles when it meets them randomally. (Blue squares)

For each particle eaten, the baterium get energy and for each movement it losts energy.
When a bacterium gets enough level of energy it would reproduce itself by dividing into two. Ech new bacterium gets half of the energy level.
On each movement the bacterium losts some energy.  When the level gets to be 0 the bacterium is died.

We can change the properties of bacteria in order to see the effect on the cleanning process.  We can as well get basic statistics
about the numbers of the bacteria , reproductions , deaths, and the particles.

**Technical Information**

This is a windows applipcation which I wrote as a graduation work for my Software Practical-Engineer degree.
It is written in **C++** with the directory of **OWL-Borland**.
The Bacteria is represented by a linked list with an iterator to treat each item using OOP methodica.

You can see more details on my **LinkedIn profile** in the project book at **"Featured"**. (IN HEBREW)
https://www.linkedin.com/in/yaakov-whise-1172322b/overlay/1635467058279/single-media-viewer/?
profileId=ACoAAAZDSSMBhzta9PrFNqktON_L4EloFiV_wQM



**Install and Run**

This is an old application which runs only on 32-bit Windows.

In order to run it on Windows 10 (maybe 11 too), do as follows :

*Download the repository files:*

Push the green **"Code"** button.

Go to your Downloads in the Files Explorer.

Right click on "Simulator-main".

Select "Extract all...".

Choose a place into which to extract the files and press Extract button.

*Download* **otvdm-v0.9.0** 

In order to run the Project.exe file :

Go to https://github.com/otya128/winevdm/releases

Download otvdm-v0.9.0.zip

Extract it the same way as "Simulator-main".

In the Files Explorer, open otvdm-v0.9.0

Open otvdmw (with w) file.

Press Ok (or Activate).

In the File Explorer window browse to the directory into which you placed the Simulator files.

Select the executive file PROJECT(.EXE).

Press Open button.

Wait 10 seconds to the openning of Simulator.

In https://www.linkedin.com/feed/update/urn:li:activity:7198410635498442754/ You can see a video for how to use (in Hebrew but the 
video is easy to understand).
