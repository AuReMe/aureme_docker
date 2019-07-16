	     
How to use Docker?
==================
+------------------------------------------------------------------------------------------------------------------+
| **Requirements** Docker (v 1.10 at least)                                                                        |
|                                                                                                                  |
| | To install Docker, please follow the instructions on `docker.com <https://www.docker.com/>`_, considering your |
| | operating system*.                                                                                             |      
|                                                                                                                  |
| * On Mac OS: requires at least Yosemite 10.10.4                                                                  |
| * On Windows: requires at least Windows 8                                                                        |
+------------------------------------------------------------------------------------------------------------------+

.. _run_docker:
   
Running a Docker container
--------------------------

1. Launch the Docker machine (see the instruction on
   `docker.com <https://www.docker.com/>`_). For example:
   
   - On Fedora: **sudo systemctl start docker**
   - On Mac OS and Windows: run the Docker launcher

2. Download the AuReMe Docker image with the command described just below.
   You will find the current **tag** of the AuReMe Docker image on this
   `page <https://hub.docker.com/r/dyliss/aureme-img/tags>`_.

.. code:: sh
	  
    shell> docker pull dyliss/aureme-img:tag

    
    
3. To verify that the image has been downloaded correctly, check it in
   the list of your local images:

.. code:: sh
	       
    shell> docker images -a
    
    REPOSITORY                            TAG      IMAGE ID       CREATED      SIZE
    docker.io/dyliss/dyliss/aureme-img    2.2      6cf38ab4edc8   1 hour ago   1.68 GB
 
4. Create a folder that will serve as a bridge to share data from/to the
   Docker container. Let us call it **bridge** for instance.

5. Create a Docker container from the following image with this command:

.. code:: sh
	  
    shell> docker run -ti -v /my/path/to/the/directory:/shared --name="aureme_cont" dyliss/aureme-img:tag bash
   
The path given for -v is the one to the shared directory on your host
machine. **This path has to end on the directory name** (without any
**/** at the end). **The path has to be complete** (from **/users**
or from **C:\\\\\\** for Windows users). After the ‘:’ is the name of
the mirror directory in the Docker container. Please do not change it.


For Windows users, be careful, you have to indicate your path this way:

You have just made a bridge between **\\my\\path\\to\\the\\directory\\brigde**
and the container **aureme_cont**.

You can create as many containers as you wish, as long as you give them
different names.

Your AuReMe container is now running and correctly installed. Congratulations!
You are now inside the container **aureme_cont**.

.. _tips_docker:

Some tips about Docker
-----------------------

- To exit the container, tape **exit.**

.. code:: sh
	  
   aureme> exit
  
- To list all your containers:

.. code:: sh
	  
   shell> docker ps -a

   CONTAINER ID   IMAGE                         COMMAND    CREATED       STATUS     PORTS   NAMES         SIZE
   fc969ed0d2c7   docker.io/dyliss/aureme-img   "bash"     2 hours ago   Up 5 hours         aureme_cont   11 MB (virtual 3.52 GB)


Remark that you can see, with this command, the state of your containers
in the STATUS column: **up** (running, you can connect to it),
**exited** (stopped, need to be started again).

- To start or stop the container (from your host):

.. code:: sh
	  
   shell> docker start aureme_cont
   shell> docker stop aureme_cont

- If you want to go inside **a started/running** container:

.. code:: sh
	  
   shell> docker exec -it aureme_cont bash
   
- To delete a container: **docker rm container_id** ( or **name**)

- To delete an image: **docker rmi image_id** (or **name**)
Before deleting a Docker image, you have to delete all the Docker containers 
which are linked with the image you would like to remove. And before removing a
Docker container, you have to stop it.
