# wallet
Local wallet (buy/sell) with coingecko api giving value.

## Project management (need to organizes ideas) :

### Architecture : 

Reflexion sur l’architecture à adopter après avoir testé les différentes parties.  

J’ai besoin de 3 éléments :  
  -  Collection de transactions.
  -  Dictionnaire des tokens.
  -  API coingecko.

#### Transactions :
  Une transaction est un ensemble de données :
    - Timestamps (64 bits).
    - ID_Key (8 bits, lié à l’ID d’un token dans le dictionnaire).
    - Token (Double 64bits, nombre de jetons obtenus(+) ou séparé(-))
    - Fiat (Double 64bits, nombre de fiat échangés).

Note : Fiat peut être de signe opposé à Token.

Les transactions sont stockés dans un fichier wallet_name.trans
Le module transactions doit :
  - Ajouter une transaction au fichier .trans
  - Parcourir les transactions pour :
    - Afficher les totaux :
        - Total fiat.
        - Nombre total de chaques token.
    - Afficher toutes les transactions d’un seul token.
  - Editer une transaction particulière (Update / del).

#### Dictionnaire

