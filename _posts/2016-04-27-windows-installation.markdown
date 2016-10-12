---
layout: default
title:  "Welcome to Jekyll!"
date:   2016-02-12 17:50:00
categories: main
---
<h1>Setting up El Maven on Windows</h1>
<p>El Maven needs a development environment to be setup in Windows before it can be installed, and MSYS does exactly that for us. MSYS is a collection of GNU utilities such as bash, make, gawk and grep to allow building of applications and programs which depend on traditionally UNIX tools to be present. It is intended to supplement MinGW and the deficiencies of the cmd shell. We build a tool chain and install all the required libraries using MSYS2. After that is done, we are ready to build Maven. The following tutorial gives step by step instructions on how to install everything you need on your Windows machine to be able to build Maven.</p>
<h3>Setting up libraries and tool chains</h3>
<p>Download the <a href="http://msys2.github.io/">MSYS2</a> installer and folow their tutorial on how to install.</p>
<p>Update the package database and pacman with:<pre>pacman -Sy</pre></p>
<p>If needed, close MSYS2, run it again from the installation folder by double clicking 'msys2.exe'. </br>Update the package database and core system packages with:
<pre>pacman -Syu</pre></p>
<p>
    Again, if needed, close MSYS2, run it again from the installation folder. Update the rest with:
<pre>pacman -Su</pre>
</p>
<p>
    Install the MSYS2 development packages:
</p>
<p>
    1. &nbsp;base-devel contains basic development tools for any building. Install it using the command:
<pre>pacman -Sy base-devel</pre>
</p>
<p>
    2. &nbsp;msys2-devel package contains tools for building msys2 packages. Install it using the command:
<pre>pacman -Sy msys2-devel</pre>
</p>
<p>
    3. &nbsp;mingw-w64-x86_64 is a toolchain used to build mingw64 packages. Install it using the command:
<pre>pacman -Sy mingw-w64-x86_64-toolchain</pre>
</p>
<p>
    4. &nbsp;mingw-w64-x86_64-qt4 - The Qt Installer Framework used for the Qt SDK installer. Install QT4 using the command:
<pre>pacman -Sy mingw-w64-x86_64-qt4</pre>
</p>
<p>
    hdf5 is a general purpose library and file format for storing scientific data. Install it using the command:
<pre>pacman -Sy mingw64/mingw-w64-x86_64-hdf5</pre>
</p>
<p>
    Interface for scientific data access to large binary data. Install it using the command:
<pre>pacman -Sy mingw64/mingw-w64-x86_64-netcdf</pre>
</p>
<p>
    To install boost libraries, use the command:
<pre> pacman -Sy mingw64/mingw-w64-x86_64-boost</pre>
</p>
<p>
    Install GIT on your system by typing the command in the MSYS2 terminal:
<pre>pacman -Sy msys/git</pre>
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
