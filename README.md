# LeafEdit
<p align="center">
  <img src="https://img.shields.io/badge/License-GPLv3-informational.svg" alt="License: GPLv3">
  <a href="https://discord.gg/KDJCfGF">
    <img src="https://img.shields.io/badge/Discord-%23leafedit-7289DA" alt="Discord Server: #leafedit-wildedit">
  </a>
  <a href="https://dev.azure.com/Universal-Team/Builds/_build?definitionId=8">
    <img src="https://dev.azure.com/Universal-Team/Builds/_apis/build/status/Universal-Team.LeafEdit?branchName=master" alt="Build status on Azure Pipelines">
  </a>
  <a href="https://crowdin.com/project/leafedit">
    <img src="https://badges.crowdin.net/leafedit/localized.svg" alt="Translation status on Crowdin">
  </a>
</p>

This is a work in progress Animal Crossing: New Leaf & Wild World Save Manager & Editor for the Nintendo 3DS!

This Project has been made with [Universal-Core](https://github.com/Universal-Team/Universal-Core).

## Citra User

To use LeafEdit completely you need to get the assets from [here](https://github.com/Universal-Team/extras/raw/master/builds/LeafEdit/assets/). Put the files to `sdmc:/3ds/LeafEdit/assets/` and you should be fine. You can also use the assets from the `resources/assets/` folder from this source.

## Compilation

## Compilation
### Setting up your enviromment

To build LeafEdit from source, you will need to setup a system with devkitARM, libctru, citro2d, citro3d, 3ds-curl and 3ds-libjpeg-turbo. Follow devkitPro's [Getting Started](https://devkitpro.org/wiki/Getting_Started) page to install pacman, then run `(sudo dkp-)pacman -S devkitARM libctru citro2d citro3d 3ds-curl 3ds-libjpeg-turbo`. You will also need [bannertool](https://github.com/Steveice10/bannertool/releases/latest) and [makerom](https://github.com/profi200/Project_CTR/releases/latest) inside the `3ds` folder.

### Cloning the repo

To download the source you will need to clone it with submodules, this can be done by running
```
git clone --recursive https://github.com/Universal-Team/LeafEdit.git
```
or if you've already cloned it running 
```
git submodule update --init --recursive
```
to update all the submodules.

### Compiling

Simply run `make` in the LeafEdit directory.


## Credits
### Main Developers
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ): App idea and main developer of LeafEdit.
### Translators

English
- [Pk11](https://github.com/Epicpkmn11)
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ)

French
- [antoine62](https://github.com/antoine62)

German
- [SuperSaiyajinStackZ](https://github.com/SuperSaiyajinStackZ)

Italian
- [edo9300](https://github.com/edo9300)

Japanese
- [Pk11](https://github.com/Epicpkmn11)

Lithuanian
- [lemonnade0](https://steamcommunity.com/profiles/76561198276444028)

Portuguese
- [Chips](https://github.com/Ch1p5)
- [David Pires](https://github.com/DavidPires)

Spanish
- [Daniyel33](https://github.com/Daniyel33)
- [XxPhoenix1996xX](https://github.com/XxPhoenix1996xX)
- [YoSoy](https://twitter.com/riku200)

### Others
- [Cuyler](https://github.com/Cuyler36), [Slattz](https://github.com/Slattz), [NLTK](https://github.com/Slattz/NLTK), [ACSE](https://github.com/Cuyler36/ACSE): For some core work of NLTK & ACSE research and for the acres, face, hair, items and villagers sprites which are available [here](https://github.com/Universal-Team/extras/raw/master/builds/LeafEdit/assets/).
- [devkitPro](https://github.com/devkitPro), [Fincs](https://github.com/fincs), [Smealum](https://github.com/smealum), [Wintermute](https://github.com/WinterMute): For devkitARM, Citro2D, Citro3D and Libctru.
- [Flame](https://github.com/FlameKat53): The name idea of LeafEdit.
- [Kodtiz3D](https://github.com/Kodtiz3D): For the icon and the banner.
- [FlagBrew](https://github.com/FlagBrew) / [piepie62](https://github.com/piepie62): For helping out by problems and [PKSM-Core](https://github.com/FlagBrew/PKSM-Core)'s Save Structure inspiration. LeafEdit's Core Structure is very similar to the one from PKSM-Core.
- [Lukas](https://github.com/RedShyGuy): For some useful AC:NL offsets for the core.
- [Pk11](https://github.com/Epicpkmn11): For being a big help.
- [TotallyNotGuy](https://github.com/TotallyNotGuy): For the amazing graphic work!
- [Universal-Updater](https://github.com/Universal-Team/Universal-Updater): Download code being ported from this app.