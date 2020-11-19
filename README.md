# LeafEdit
<p align="center">
  <img src="https://img.shields.io/badge/License-GPLv3-informational.svg" alt="License: GPLv3">
  <a href="https://discord.gg/KDJCfGF">
    <img src="https://img.shields.io/badge/Discord-%23animal--crossing-7289DA" alt="Discord Server: #leafedit-wildedit">
  </a>
  <a href="https://github.com/Universal-Team/LeafEdit/actions?query=workflow%3A%22Build+LeafEdit%22">
   <img src="https://github.com/Universal-Team/LeafEdit/workflows/Build%20LeafEdit/badge.svg" height="20" alt="Build status on GitHub Actions">
  </a>
  <a href="https://crowdin.com/project/leafedit">
    <img src="https://badges.crowdin.net/leafedit/localized.svg" alt="Translation status on Crowdin">
  </a>
</p>

LeafEdit is a work in progress save editor and save manager for the following games:
* Animal Crossing: Wild World
* Animal Crossing: New Leaf
* Animal Crossing: New Leaf Welcome amiibo
* Animal Crossing: Happy Home Designer

This Project has been made with [Universal-Core](https://github.com/Universal-Team/Universal-Core) and [LeafEdit-Core](https://github.com/Universal-Team/LeafEdit-Core).


## Redirections
- For a list of all editable features as of latest Nightly, look [here](https://github.com/Universal-Team/LeafEdit/blob/master/editing-features.md).

## Citra User
To use LeafEdit completely you need to get the assets from [here](https://github.com/Universal-Team/extras/raw/master/builds/LeafEdit/assets/). Put the files to `sdmc:/3ds/LeafEdit/assets/` and you should be fine.

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

## Screenshots about LeafEdit
![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/acreEditorNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/acreEditorWW.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/appearanceEditorNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/appearanceEditorWW.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/badgeEditor.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/credits.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/developed_screen.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/editor.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/itemEditorNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/itemEditorWW.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/itemList.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/mainmenu.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/mapEditorNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/mapEditorWW.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/paletteToolNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/paletteToolNL2.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/paletteToolWW.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/patternEditor.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/patternToolMenu.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/patternViewer.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/playerEditorNL.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/playerEditorNL2.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/playerEditorWW.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/playerEditorWW2.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/playerSelector.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/pocketEditorNL.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/pocketEditorWW.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/saveSelect.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/scriptMain.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/settings.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/townEditor.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/updateCenter.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/villagerEditor.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/villagerItemEditor.png)

![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/villagerSelect.png) ![](https://github.com/Universal-Team/LeafEdit/blob/master/Screenshots/villagerViewer.png)


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
- [Cuyler36](https://github.com/Cuyler36), [Slattz](https://github.com/Slattz), [NLTK](https://github.com/Slattz/NLTK), [ACSE](https://github.com/Cuyler36/ACSE): For the acres, face, hair, items and villagers sprites which are available [here](https://github.com/Universal-Team/extras/raw/master/builds/LeafEdit/assets/).
- [devkitPro](https://github.com/devkitPro): For devkitARM, Citro2D, Citro3D and Libctru.
- [FlagBrew](https://github.com/FlagBrew) / [piepie62](https://github.com/piepie62): For helping out by problems and [PKSM-Core](https://github.com/FlagBrew/PKSM-Core)'s Save Structure inspiration. LeafEdit's Core Structure is very similar to the one from PKSM-Core.
- [Flame](https://github.com/FlameKat53): The name idea of LeafEdit.
- [Kodtiz3D](https://github.com/Kodtiz3D): For the icon and the banner.
- [LeafEdit-Core](https://github.com/Universal-Team/LeafEdit-Core): The save editing code comes from here. Credits to all the contributors from LeafEdit-Core.
- [Pk11](https://github.com/Epicpkmn11): For being a big help.
- [RedShyGuy](https://github.com/RedShyGuy): For some useful AC:NL offsets for the core.
- [TotallyNotGuy](https://github.com/TotallyNotGuy): For the amazing graphic work!
- [Universal-Updater](https://github.com/Universal-Team/Universal-Updater): Download code being ported from this app.
