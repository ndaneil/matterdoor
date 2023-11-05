# MatterDoor android app

This code is a fork of [Google Home Mobile SDK Sample Applications for Matter](https://github.com/google-home/sample-apps-for-matter-android/tree/main). I made modifications so that the three states (`OFF`, `FREE`, `BUSY`) can be controlled from the application. Additionally, I created a new logo for the application along with changes to the underlying logic so that instead of using an on/off endpoint, a level endpoint is used for the state of the MatterDoor device. 

The MatterDoor device can be commissioned to this app's Matter fabric and can be shared as well.


## Clone the repository

The Sample app GitHub repository includes third party libraries from the
[Matter repo (`connectedhomeip`)](https://github.com/project-chip/connectedhomeip).
These native libraries are over 50MB, and require the use of Git Large File
Storage (LFS).

To clone the repository, complete the following steps:

1.  Install [Git LFS](https://git-lfs.github.com/).

2.  Initialize Git LFS.

    ```console
    $ git lfs install
    ```

    When complete, the console displays the following:

    ```console
    Updated Git hooks.
    Git LFS initialized.
    ```

3.  Once Git LFS is installed and initialized, you're ready to clone the
    repository. When cloning completes, Git checks out the `main` branch
    and downloads the native libraries for you.

    ```console
    $ git clone https://github.com/ndaneil/matterdoor.git
    ```

## Original License

The original Google Home Sample App for Matter is licensed under
the [Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0).

[As required by the terms of the license, a copy of the orignal terms is provided.](../original-LICENSE)
