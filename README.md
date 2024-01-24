# Sabres Path Validation

Sabres Path Validation utilizes Zero-Knowledge Proofs to ensure path routes are
followed, and ensure that no path leakage occurs beyond next-hop neighbors.

## Building

The current method for building the repo is through docker.  The Dockerfile
contains the dependencies and set of instructions required to build the code.

```
sudo docker build -t path_validation:latest -f Dockerfile .
```

## Testing Path Validation

To test the path validation code, we use docker-compose to create a virtual
network of containers. One can increase the scale of testing by passing the
`--scale` argument with `sabres_node` to increase the number of containers
used in the test (e.g., 4 nodes in a network).

```
docker-compose up -d --scale sabres_node=4
```

This command will also generate a private internal network named
`sabres_internal_network`. The following command will help in checking the
network status.

```
docker network inspect sabres_internal_network
```

We'll need to check what internal ports have been mapped to the host machine.
Using `docker ps` to see `0.0.0.0:52396->3000/tcp` indicates that an internal
port of 52396 has been mapped to 3000 on the host machine.


To tear down the docker compose environment use `down`.
```
docker-compose down
```

### Node API
* \[POST\] [/initialize](#initialize)
* \[POST\] [/genproof](#generate-hash-and-proof)
* \[POST\] [/verifyproof](#verify-proof)
* \[POST\] [/sendData](#send-data)
* \[POST\] [/sendContent](#send-content)
* \[POST\] [/chainIdeal](#ideal)
* \[POST\] [/NIZK](#xor-hash-nizk)
* \[POST\] [/xorhash](#xorhash)
* \[GET\] [/info](#information)
* \[GET\] [/dataList](#show-data-list)
* \[GET\] [/clearData](#clear-data-list)

#### Initialize
It will generate secrets (below we generate 5 secrets).

```
curl --location --request POST '127.0.0.1:52090/initialize' \
--header 'Content-Type: application/json' \
--data-raw '{
    "number": 5
}'

```
#### Generate Hash and Proof
If only given first number, then it will generate hash without xor operation.
Hash file's name will be as same as the first number's name.

```
curl --location --request POST '127.0.0.1:52090/genproof' \
--header 'Content-Type: application/json' \
--data-raw '{
    "first_number": 2,
    "second_number": 3
}'
```

#### Verify Proof
```
curl --location --request POST '127.0.0.1:52090/verifyproof' \
--header 'Content-Type: application/json' \
--data-raw '{
    "number": 5
}'
```

#### Send Data
We can use the container's name as the url in the network.

```
curl --location --request POST '127.0.0.1:52090/sendData' \
--header 'Content-Type: application/json' \
--data-raw '{
    "url": "sabres_sabres_node_2",
    "filename": "2.secret"
}'
```

#### Ideal World Functionality
Sending data from some node to the receiver node.
Because the node will deliver the data by fixed path (e.g. 1, 2, 3, 4),
so it doesn't have to provide the url.

```
curl --location --request POST '127.0.0.1:52090/chainIdeal' \
--header 'Content-Type: application/json' \
--data-raw '{
    "filename": "1.secret"
}'
```

#### XOR-Hash Protocol
Sending data from some node to the receiver node.
Because the node will deliver the data by fixed path (e.g. 1, 2, 3, 4),
so it doesn't have to provide the url.

```
curl --location --request POST '127.0.0.1:52090/sendContent' \
--header 'Content-Type: application/json' \
--data-raw '{
    "filename": "new.secret"
}'
```

#### XOR-Hash-NIZK
Sending data from some node to the receiver node.
Because the node will deliver the data by fixed path (e.g. 1, 2, 3, 4),
so it doesn't have to provide the url.

```
curl --location --request POST '127.0.0.1:52090/NIZK' \
--header 'Content-Type: application/json' \
--data-raw '{
    "filename": "0.secret"
}'
```

#### Xorhash
Given two file names, make them xor together and hash the new secret.

```
curl --location --request POST '0.0.0.0:52090/xorhash' \
--header 'Content-Type: application/json' \
--data-raw '{
    "first_secret": "4.secret",
    "second_secret": "5.secret"
}'
```

#### Information
Will return node hostname and the number of node in the network

```
curl --location --request GET '127.0.0.1:52090/info'
```

#### Show Data List

```
curl --location --request GET '127.0.0.1:52090/dataList'
```

#### Clear Data List

```
curl --location --request GET '127.0.0.1:52090/clearData'
```

### Exception Warning
Sometimes, proof generating goes wrong. When an error occurred, the proof file
will not be generated due to internal error. Hence, you can check the data list
for making sure each step is correct in the progress.
