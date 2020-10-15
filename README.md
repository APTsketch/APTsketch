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

#### Move to GenMixData dictionary

```shell
cd GenMixData
```

#### Modify the GenData.cpp

The following parameters can be adjusted.

* comment, which identifies the output file
* normalFlowDir, which is the position of background flow
* attackFlowDir, which is the position of APT flow

#### Edit the compile.sh

The usage of sim.out is:

```shell
./sim.out dilutionFactor
```

For simulated datasets, the dilutionFactor should be 1.

For mixed datasets, the dilutionFactor can be adjusted.

#### Generate datasets

```shell
./compile.sh
```

The output file is in *data/mix/*, whose name is *yourComment*.dat

## Mark ground truth

#### Move to MarkAPT dictionary

```shell
cd MarkAPT
```

#### Modify the MarkAPT.cpp

The following parameters can be adjusted.

* normalFlowDir, which is the position of simulated/mixed dataset
* attackListDir, which is the position of output ground truth

#### Edit the compile.sh

The usage of mark.out is:

```shell
./mark.out LT RT minDuration maxBatchSize
```

where

* LT represents $$\mathcal{T}_l$$
* RT represents $$\mathcal{T}_r$$
* minDuration represents $$\mathcal{T}_d$$
* maxBatchSize represents $$\mathcal{T}_s$$

#### Mark ground truth

```shell
./compile.sh
```

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