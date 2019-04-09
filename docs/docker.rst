	     
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

2. Download the AuReMe Docker image
   ::
    shell> docker pull dyliss/aureme-img
    
3. To verify that the image has been downloaded correctly, check it in
   the list of your local images:
   ::
    shell> docker images -a
    
    REPOSITORY           TAG      IMAGE ID       CREATED      SIZE
    dyliss/aureme-img    latest   6cf38ab4edc8   1 hour ago   1.68 GB
 
4. Create a folder that will serve as a bridge to share data from/to the
   Docker container. Let us call it **bridge** for instance.

5. Create a Docker container from the following image with this command:
   ::
    shell> docker run -ti -v /my/path/to/the/directory:/shared --name="aureme_cont" dyliss/aureme-img bash
   
The path given for -v is the one to the shared directory on your host
machine. **This path has to end on the directory name** (without any
**/** at the end). **The path has to be complete** (from **/users**
or from **C:\\\\\\** for Windows users). After the ‘:’ is the name of
the mirror directory in the Docker container. Please do not change it.


For Windows users, be careful, you have to indicate your path this way:

You have just made a bridge between **\\my\\path\\to\\the\\directory\\brigde**
and the container **aureme-cont**.

You can create as many containers as you wish, as long as you give them
different names.

Your AuReMe container is now running and correctly installed. Congratulations!
You are now inside the container **aureme-cont**.

Some tips about Docker
-----------------------

- To exit the container, tape **exit.**
  ::
   aureme> exit
  
- To list all your containers:
  ::
   shell> docker ps -a

   CONTAINER ID   IMAGE        COMMAND    CREATED       STATUS     PORTS   NAMES
   fc969ed0d2c7   aureme-img   "bash"     2 hours ago   Up 5 hours         aureme-cont

Remark that you can see, with this command, the state of your containers
in the STATUS column: **up** (running, you can connect to it),
**exited** (stopped, need to be started again).

- To start or stop the container (from your host):
  ::
   shell> docker start aureme-cont
   shell> docker stop aureme-cont

- If you want to go inside **a started/running** container:
  ::
   shell> docker exec -it aureme-cont bash
   
- To delete a container: **docker rm container_id** ( or **name**)

- To delete an image: **docker rmi image_id** (or **name**)
Before deleting a Docker image, you have to delete all the Docker containers 
which are linked with the image you would like to remove. And before removing a
Docker container, you have to stop it.
