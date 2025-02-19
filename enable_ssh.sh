#!/bin/sh

set -e

# The local network IP address of the printer to connect to
IP=$1

# The location of a public key to use for ssh access
PUBKEY="$HOME/.ssh/id_rsa.pub"

# Size and CRC for the public key are required to transfer it
SIZE=$(stat -c %s $PUBKEY)
CRC=$(printf '%d' 0x$(crc32 $PUBKEY))

# Request authorization and then send commands to enable ssh.  This command will block
# until the printer UI is used to confirm that access to the printer is authorized
echo "Please authorize access on the printer UI"
NONCE=$(head -c 15 /dev/urandom | base64 | tr / _)
echo "{\"jsonrpc\":\"2.0\",\"method\":\"authorize\",\"id\":0,\"params\":[\"ssh\",null,\"$NONCE\"]}" >/tmp/$NONCE
tail --follow=name /tmp/$NONCE 2>/dev/null | openssl s_client -connect $IP:12309 2>/dev/null | (awk 'BEGIN{RS="[{},]"};/"error"/{exit 1};/"one_time_token"/{exit 0}' && echo "{\"jsonrpc\":\"2.0\",\"method\":\"put_init\",\"id\":2,\"params\":[\"/id_rsa.pub\",0,$SIZE,$SIZE]}" >>/tmp/$NONCE && sleep 1 && echo -n "{\"jsonrpc\":\"2.0\",\"method\":\"put_raw\",\"id\":3,\"params\":[0,$SIZE]}" >>/tmp/$NONCE && cat $PUBKEY >>/tmp/$NONCE && echo "{\"jsonrpc\":\"2.0\",\"method\":\"put_term\",\"id\":4,\"params\":[0,$SIZE,$CRC]}" >>/tmp/$NONCE && sleep 1 && echo "{\"jsonrpc\":\"2.0\",\"method\":\"copy_ssh_id\",\"id\":5,\"params\":[\"/home/id_rsa.pub\"]}" >>/tmp/$NONCE && sleep 1 && echo "SSH access enabled!" || echo "Authorization failed"; rm /tmp/$NONCE)
