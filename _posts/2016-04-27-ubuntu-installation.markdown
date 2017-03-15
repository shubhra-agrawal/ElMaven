---
layout: default
title:  "Welcome to Jekyll!"
date:   2016-02-12 17:50:00
categories: main
---
<h1>Setting up El Maven on Ubuntu</h1>
<p>Installing Maven in Ubuntu invloves intalling QT5 and a few libraries which allow for signalling and handling scientific data.
This tutorial covers how to install these libraries and then using them to launch Maven in Ubuntu.
</p>

<h3>Setting up libraries and tool chain</h3>
<p>The libraries and tool required are mentioned below:</p>
<p>1.  &nbsp;QT</p>
<p>To install QT5 development tools in Ubuntu use the command:</p>
<pre>sudo apt-get install qt5-default qttools5-dev-tools</pre>



<p>2.  &nbsp;Boost Libraries</p>
<p>To install boost libraries use the command:</p>
<pre>sudo apt-get install libboost-all-dev</pre>



<p>3.  &nbsp;zlib</p>
<p>Go to the <a href="http://www.zlib.net/">link</a> and download the zlib source code in tar.gz format</p>

<p>Open the terminal and navigate to where the source code was downloaded:</p>
<pre>cd &#60;pathtofolder&#62;</pre>
For Example: cd /home/elucidata/Downloads

<p>Extract the compressed file using the command:</p>
<pre>tar -xzvf zlib-1.2.8.tar.gz</pre>

<p>Navigate to the extracted folder using cd:</p>
<pre>cd zlib-1.2.8</pre>

<p>Configure zlib using the command:</p>
<pre>./configure</pre>

<p>Then use the following commands to install zlib:</p>
<pre>make</pre>
<pre>sudo make install</pre>



<p>4.  &nbsp;hdf5 </p>
<p>Go to the <a href="https://www.hdfgroup.org/ftp/HDF5/current/src/">link</a> and download the hdf5 source code in tar.gz format</p>

<p>Open the terminal and navigate to where the source code was downloaded:</p>
<pre>cd &#60;pathtofolder&#62;</pre>
For Example: cd /home/elucidata/Downloads

<p>Extract the compressed file using the command:</p>
<pre>tar -xzvf hdf5-1.2.8.tar.gz</pre>

<p>Navigate to the extracted folder using cd:</p>
<pre>cd hdf5-1.2.8</pre>

<p>Configure hdf5 using the command:</p>
<pre>./configure</pre>

<p>Then use the following commands to install hdf5:</p>
<pre>make</pre>
<pre>sudo make install</pre>



<p>5.  &nbsp; netcdf</p>
<p>Go to the <a href="http://www.unidata.ucar.edu/downloads/netcdf/index.jsp">link</a> and download the netcdf source code in tar.gz format</p>
<p>Open the terminal and navigate to where the source code was downloaded:</p>
<pre>cd &#60;pathtofolder&#62;</pre>
For Example: cd /home/elucidata/Downloads

<p>Extract the compressed file using the command:</p>
<pre>tar -xzvf netcdf-4.4.0.tar.gz</pre>

<p>Navigate to the extracted folder using cd:</p>
<pre>cd netcdf-4.4.0</pre>

<p>Configure netcdf using the command:</p>
<pre>./configure --disable-netcdf4</pre>

<p>Then use the following commands to install netcdf:</p>
<pre>make</pre>
<pre>sudo make install</pre>

<p>Now all the required libraries for Maven have been installed.</p>

<h3>Building El Maven</h3>
<p>Create a folder in Ubuntu. Navigate to that folder using cd command:</p>

<pre>cd &#60;pathtofolder&#62;    #for example: cd /home/elucidata/Desktop</pre>
<p>To clone the master branch of the remote repository, use the following command:</p>

<pre>git clone https://github.com/ElucidataInc/ElMaven.git</pre>
<p>Now you have the latest version of Maven in the folder you made.</p>

<p>To build Maven, run the script, run.sh using the command</p>

<pre>./run.sh</pre>
<p>If it compiles successfully, it will create an executable in the bin folder. To launch maven by running the executable, use the command:</p>

<pre>./bin/El_Maven_0.1</pre>
<p>To uninstall Maven, use the command:</p>

<pre>./uninstall</pre>
