# APT Sketch

The implementation of APT Sketch, strawman solution, and generation of datasets and ground truth.

## Generate datasets

Generated datasets are recommanded to put in *./data* dictionary, where the background flows are put in *./data/normal*, the APT flows are put in *./data/attack*, and the simulated/mixed datasets are put in *./data/mix*. 

The following shows three steps to generate one dataset.

### 1. generate simulated background flows

### 2. generate redundancy of repeated APT flows

#### Move to GenAtkData dictionary

```shell
cd GenAtkData
```

#### Modify the GenAtkData.cpp

The following parameters can be adjusted.

* attackFlowDir, which is the position of APT flow
* comment, which identifies the output file

#### Edit the compile.sh

The usage of sim.out is:

```shell
./sim.out insertedNum maxOffset
```

we recommand to set maxOffset = 20000000

#### Generate datasets

```shell
./compile.sh
```

The output file is in *data/mix/*, whose name is *yourComment*_*insertedNum*.dat

### 3. generate simulated/mixed datasets



## Mark ground truth

## APT Sketch

#### Move to APT Sketch dictionary

```shell
cd APTsketch
```

#### Modify the *demo.cpp*

The following parameters can be adjusted.

* LT, which represents $$\mathcal{T}_l$$
* RT, which represents $$\mathcal{T}_r$$
* MinKeep, which is set to be equal to RT if *hot flow protection* is used, and is set to be 0 otherwise
* OUTTHRES, which represents $$\mathcal{T}_{report}$$
* testDir, which is the position of the test dataset
* attackListDir, which is the position of ground truth file of the test dataset

#### Edit the *compile.sh*

The usage of demo.out is: 

```shell
./demo.out bucketNum cellNum
```

If the cellNum is not 8, then Macros CELLNUM in cell.h should be edited as well.

#### Run the APT sketch

```shell
./compile.sh
```

## strawman solution

#### Move to strawman dictionary

```shell
cd strawman
```

#### Modify the demo.cpp

The following parameters can be adjusted.

* LT, which represents $$\mathcal{T}_l$$
* RT, which represents $$\mathcal{T}_r$$
* OUTTHRES, which represents $$\mathcal{T}_{report}$$
* testDir, which is the position of the test dataset
* attackListDir, which is the position of ground truth file of the test dataset

#### Edit the compile.sh

The usage of demo.out is:

```shell
./demo.out bucketNum cellNum
```

If the cellNum is not 8, then Macros CELLNUM in cell.h should be modified as well.

#### Run the strawman solution

```shell
./compile.sh
```