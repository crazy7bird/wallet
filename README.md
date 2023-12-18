# wallet
Local wallet (buy/sell) with coingecko api giving value.

## File system 
for a minimal footprint data i have to know :

- Coingecko token used, by the wallet.
- Each token as a token_id wich is recognized by coingecko, and an ID wich is used by the wallet.
- I need to save, the token_id wich is a char\*.
- Token ID is set by the program with the postion of the token_id in the list.
- I need to store transactions : 
    - 64 bits Timestamps
    - uint8_t ID (linked to the token_id position)
    - double token (postif for buy negative for sell).
    - double fiat spend or received.

I’ll store everything in one bin file with a header that is the list of used token, then all transactinos :

wallet.bin :
> bitcoin\0ethereum\0…token\0\0
> transaction_1
> transaction_2
> …
> tranaction_n

