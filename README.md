# sabres

Sabres Network is a simulated peer to peer network. The system try to use Zero-Knowledge Proof to protect the privacy of a network path.

### Build a Network
you can assign nodes number through passing argument (e.g. 4 nodes in a network).
```
docker-compose up -d --scale sabres_node=4
```
This command will also generate a private internal network named `sabres_internal_network`. you can use command to check the network status.
```
docker network inspect sabres_internal_network
```
Checking mapped ports for invoking api later. (e.g. 0.0.0.0:52396->3000/tcp means the mapped port is 52396, then you should use this port to invoke the node api)
```
$ docker ps
```
If you want to remove the network
```
docker-compose down
```

### Node API
* \[POST\] [/initialize](#initialize)
* \[POST\] [/genproof](#generate-hash-and-proof)
* \[POST\] [/verifyproof](#verify-proof)
* \[POST\] [/sendData](#send-data)
* \[GET\] [/dataList](#show-data-list)
* \[GET\] [/clearData](#clear-data-list)
#### Initialize
It will generate secrets
(e.g. generates 5 secrets)
```
curl --location --request POST '127.0.0.1:52090/initialize' \
--header 'Content-Type: application/json' \
--data-raw '{
    "number": 5
}'
```
#### Generate Hash and Proof
```
curl --location --request POST '127.0.0.1:52090/genproof' \
--header 'Content-Type: application/json' \
--data-raw '{
    "number": 5
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
We can use container_name as the url in the network.
```
curl --location --request POST '127.0.0.1:52090/sendData' \
--header 'Content-Type: application/json' \
--data-raw '{
    "url": "sabres_sabres_node_2",
    "filename": "2.secret",
}'
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
Sometimes, proof generating goes wrong. When an error occurred, the proof file will not be generated due to internal error. Hence, you can check the data list for making sure each step is correct in the progress.