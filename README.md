# APT Sketch

The implementation of APT Sketch, strawman solution, and generation of datasets and ground truth.

## Generate datasets

Generated datasets are recommanded to put in *./data* dictionary, where the background flows are put in *./data/normal*, the APT flows are put in *./data/attack*, and the simulated/mixed datasets are put in *./data/mix*. 

The following shows three steps to generate one dataset.

### 1. generate simulated background flows

### 2. generate redundancy of repeated APT flows

### 3. generate simulated/mixed datasets

## Mark ground truth

## APT Sketch

##### Move to APT Sketch dictionary.

```
cd APTsketch
```

##### Modify the *demo.cpp*

The following parameters can be adjusted.

* LT, which represents $$\mathcal{T}_l$$
* RT, which represents $$\mathcal{T}_r$$
* MinKeep, which is set to be equal to RT if *hot flow protection* is used, and is set to be 0 otherwise
* OUTTHRES, which represents $$\mathcal{T}_{report}$$
* testDir, which is the position of the test dataset
* attackListDir, which is the position of ground truth file of the test dataset

##### Edit the *compile.sh*

The usage of demo.out is: 

```
./demo.out bucketNum cellNum
```

If the cellNum is not 8, then Macros CELLNUM in cell.h should be edited as well.

##### Run the APT sketch

```
./compile.sh
```

## strawman solution

##### Move to strawman dictionary

```
cd strawman
```

##### Modify the demo.cpp

The following parameters can be adjusted.

* LT, which represents $$\mathcal{T}_l$$
* RT, which represents $$\mathcal{T}_r$$
* OUTTHRES, which represents $$\mathcal{T}_{report}$$
* testDir, which is the position of the test dataset
* attackListDir, which is the position of ground truth file of the test dataset

##### Edit the compile.sh

The usage of demo.out is:

```
./demo.out bucketNum cellNum
```

If the cellNum is not 8, then Macros CELLNUM in cell.h should be modified as well.

##### Run the strawman solution

```
./compile.sh
```