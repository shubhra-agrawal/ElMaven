---
layout: default
title:  "Welcome to Jekyll!"
date:   2016-02-12 17:50:00
categories: main
---
<h1>Installing El-Maven</h1>
<h3>Follow the steps to install El-Maven on 64bit Windows system:</h3>
<p>1. Double click on the installer and click Next</p>
![El-Maven Setup](https://dl.dropboxusercontent.com/s/t38t3jm1spc86hp/1.PNG?dl=0){:class="img-responsive"}
<p>2. Choose the path where you want to install El-Maven and then click Next</p>
![El-Maven Setup](https://dl.dropboxusercontent.com/s/elanm23f0f4pemc/2.PNG?dl=0){:class="img-responsive"}
<p>3. Click Next to start Installing. It'll take less than 30 seconds to install</p>
![El-Maven Setup](https://dl.dropboxusercontent.com/s/apkiw057kx52kbc/3.PNG?dl=0){:class="img-responsive"}
![El-Maven Setup](https://dl.dropboxusercontent.com/s/b8ssmmlvmjqpe14/4.PNG?dl=0){:class="img-responsive"}
<p>4. Click finish to end the Installation. A shortcut is made on the Desktop. Now you can start using El-Maven</p>
![El-Maven Setup](https://dl.dropboxusercontent.com/s/em2d4dtk59k3f61/5.PNG?dl=0){:class="img-responsive"}
<h1>Setting up El-Maven on Windows 64-Bit</h1>
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
<pre>pacman --force -Sy mingw-w64-x86_64-toolchain</pre>
</p>
<p>
    4. &nbsp;mingw-w64-x86_64-qt4 - The Qt Installer Framework used for the Qt SDK installer. Install QT4 using the command:
<pre>pacman --force -Sy mingw-w64-x86_64-qt4</pre>
</p>
<p>
    hdf5 is a general purpose library and file format for storing scientific data. Install it using the command:
<pre>pacman --force -Sy mingw64/mingw-w64-x86_64-hdf5</pre>
</p>
<p>
    Interface for scientific data access to large binary data. Install it using the command:
<pre>pacman --force -Sy mingw64/mingw-w64-x86_64-netcdf</pre>
</p>
<p>
    To install boost libraries, use the command:
<pre> pacman --force -Sy mingw64/mingw-w64-x86_64-boost</pre>
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
<p><pre>git clone https://github.com/ElucidataInc/ElMaven.git</pre></p>

<p>Now you have the latest version of Maven in the folder you made.</p>
<p>To build Maven, run the script, run.sh using the command</p>
<pre>./run.sh</pre>

<p>If it compiles successfully, it will create an executable in the bin folder. To launch maven by running the executable, use the command:
</p>
<pre>./bin/El_Maven_0.1</pre>

<p>To uninstall Maven, use the command:</p>
<pre>./uninstall</pre>
