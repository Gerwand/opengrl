# Development plan for project

Hi, dear fella! \
If your name is not Michał and it's not your project, I don't think you should
be there... \
If so, please contact me at: michal7650@gmail.com.

This is just development plan for keeping up track of my Master Of Science
progress and to plan further steps. \
The phase one for the project is done. Now it needs phase two with a lot of
additional work.

We will split this into 2 sections: harmonogram table and list with detailed
description of the plan.

So let's go!

## Harmonogram

| Task name | Time | Type of task | Link | Is done | When done |
|----------:|:--------------:|:------------:|:----:|:-------:|-----------|
| Basic documentation    | 2h  | Documentation | [Go!](#bdoc)     | [ ] | - |
| Fix the project        | 6h  | Configuration | [Go!](#pfix)     | [ ] | - |
| Make project portable  | 3h  | Configuration | [Go!](#portable) | [ ] | - |
| Reorganize project     | 6h  | Structure     | [Go!](#reorg)    | [ ] | - |
| Merge Linux trainer    | 2h  | Code          | [Go!](#lmerge)   | [ ] | - |
| Remove redundant files | 3h  | Code          | [Go!](#fclean)   | [ ] | - |
| Code cleanup           | 20h | Code          | [Go!](#pclean)   | [ ] | - |
| Skeleton approximation | 5h  | Code          | [Go!](#skel)     | [ ] | - |
| Compare joints         | 8h  | Code          | [Go!](#jcmp)     | [ ] | - |
| Fix comparing tracks   | 6h  | Test          | [Go!](#tcmp)     | [ ] | - |
| Live test              | ?   | Test          | [Go!](#integr)   | [ ] | - |
| Final documentation    | 12h | Documentation | [Go!](#doc)      | [ ] | - |
| GPU offload            | 16h | Code          | [Go!](#gpu)      | [ ] | - |
| Neural networks        | 24h | Code          | [Go!](#ai)       | [ ] | - |
| Improve GUI            | 12h | Code          | [Go!](#gui)      | [ ] | - |

## Tasks

Here is the list of minimum required effort to complete the project.

### <a name="bdoc"></a> Basic documentation for the project

The project is missing documentation. It should describe:

1. Dependencies
1. How to setup and build
1. List and description of all subprojects
1. Usage guide (for now, overview only)

More?

### <a name="pfix"></a> Fix the project

The project is now broken - it is probably missing dependencies, there is code
that is not building etc.

It should be fixed, invalid code should be commented and appropriate comments
in the code should be added.

### <a name="portable"></a> Make project portable

The paths in the project configuration are absolute.
They should:

1. Use ENV variables
1. Use common paths for opencv etc
1. Include somewhere archive with static and dynamic variables
1. Take into consideration using static libraries
1. DLLs should be included somewhere

### <a name="reorg"></a> Reorganize project

The files are in single folder, shared files are belonging to invalid projects
etc.

To fix that, file reorganization is needed:

1. All common source files should be in the Source folder
2. They can be further divided into categories
3. Sources specific to the project can stay in the project folder
4. The projects should have fixed links to the files (reconfigure)

### <a name="lmerge"></a> Merge Linux trainer

The Linux trainer was hacked to be working with OpenMP.

Those changes should be integrated to the main repo and it should replace
RDF trainer.

Moreover, consider making the app being cross platform (Windows and Linux).

### <a name="fclean"></a> Remove redundant files

All files were included to the git, however, some of there were backups and
some old changes. For now they were not checked, but they should be removed
so the repo will be smaller and tidier.

### <a name="pclean"></a> Code cleanup

Review whole code and maybe reorganize/rewrite classes a bit. It will be hard
effort, but it may be worth to do that.

### <a name="skel"></a> Skeleton approximation

Approximate skelton from classes in the image. This is probably done, but need
to make sure...

### <a name="jcmp"></a> Compare joints

Write algorithm that will be able to compare joints. Maybe for now, the simple
KNN will be good, but in the future, let's consider neural networks.

### <a name="tcmp"></a> Fix comparing tracks

There are some issues with comparing tracks.

Let's create unit tests, prepare some simple test data and check if the algorithm is working fine. Debugging FTW!

### <a name="integr"></a> Live test

Integrate the code with the GUI and check if it works

### <a name="gpu"></a> GPU offload

Let's try to offload something to GPU, like RDF training or classification.

### <a name="ai"></a> Neural networks

Try to use AI for the track comparison.

### <a name="gui"></a> Improve GUI

The GUI will be missing some settings, importing database, chosing method of
computations, algorithms etc. It should be just more interactive

### <a name="doc"></a> Final documentation of the project

By now, almost everything should be done. Full usage guide should be written
and the code should be documented.
