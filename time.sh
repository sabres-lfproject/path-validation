#curl -d "data=@auth.txt" http://sabres_sabres_node_1:3000/upload
curl --location --request POST 'http://sabres_sabres_node_1:3000/sendData' \
--header 'Content-Type: application/json' \
--data-raw '{
    "url": "sabres_sabres_node_2",
    "filename": "ideal.secret"
}'

i=$(date +%s%N)
echo "$i" >> time-ideal.txt
