---
layout: default
title:  "Welcome to Jekyll!"
date:   2016-02-12 17:50:00
categories: main
---
<h1>Setting up El Maven on Windows 32-Bit</h1>
<h3>Setting up libraries and tool chains</h3>
<p>Update the package database and pacman with:<pre>pacman --force -Sy</pre></p>
<p>If needed, close MSYS2, run it again from the installation folder by double clicking 'msys2.exe'. </br>Update the package database and core system packages with:
<pre>pacman --force -Syu</pre></p>
<p>
    Again, if needed, close MSYS2, run it again from the installation folder. Update the rest with:
<pre>pacman --force -Su</pre>
</p>
<p>
    Install the MSYS2 development packages:
</p>
<p>
    1. &nbsp;base-devel contains basic development tools for any building. Install it using the command:
<pre>pacman --force -Sy base-devel</pre>
</p>
<p>
    2. &nbsp;msys2-devel package contains tools for building msys2 packages. Install it using the command:
<pre>pacman --force -Sy msys2-devel</pre>
</p>
<p>
    3. &nbsp;mingw-w64-x86_64 is a toolchain used to build mingw64 packages. Install it using the command:
<pre>pacman --force -Sy mingw-i686-toolchain</pre>
</p>
<p>
    4. &nbsp;mingw-w64-x86_64-qt4 - The Qt Installer Framework used for the Qt SDK installer. Install QT4 using the command:
<pre>pacman --force -Sy mingw-i686-qt4</pre>
</p>
<p>
    hdf5 is a general purpose library and file format for storing scientific data. Install it using the command:
<pre>pacman --force -Sy mingw32/mingw-i686-hdf5</pre>
</p>
<p>
    Interface for scientific data access to large binary data. Install it using the command:
<pre>pacman --force -Sy mingw32/mingw-i686-netcdf</pre>
</p>
<p>
    To install boost libraries, use the command:
<pre> pacman --force -Sy mingw32/mingw-i686-boost</pre>
</p>
<p>
    Install GIT on your system by typing the command in the MSYS2 terminal:
<pre>pacman --force -Sy msys/git</pre>
</p>
<p>
    This completes the installation of MSYS2 and the tool chain required to build Maven on your computer.
</p>
<h3>Building El Maven</h3>
<p>In the folder where MSYS2 was installed, double click on mingw64.exe and type in the following:</p>
<p>Create a folder on Windows. Navigate to that folder using cd command:</p>
<p><pre>cd &#60;pathtofolder&#62;    #for example: cd /c/User/Admin/Desktop</pre>
</p>

<p>To clone the master branch of the remote repository, use the following command: </p>
<p><pre>git clone https://github.com/sabsgeo/nonlinear-optimization</pre></p>

<p>Now you have the latest version of Maven in the folder you made.</p>
<p>To build Maven, run the script, run.sh using the command</p>
<pre>./run.sh</pre>

<p>If it compiles successfully, it will create an executable in the bin folder. To launch maven by running the executable, use the command:
</p>
<pre>./bin/El_Maven_0.1</pre>

<p>To uninstall Maven, use the command:</p>
<pre>./uninstall</pre>
