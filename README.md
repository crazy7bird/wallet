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
Le dictionnaire contient les token utilisés dans le wallet.
Il détermine l’ ID_Key d’un token, qui est utilisé dans les transaction.
Une entrée du dictionnaire contient :
  - ID_Key (peut être détermiré par la position dans le dictionnaire).
  - Token_id (char* token_id utilisé par coingecko pour retrouver un token).
  - Token_short (char* short du token ex bitcoin => btc, en corrélation avec coingecko api).
  - Token_name (char* Token name de coingecko).
  - Current_price (double, price given by coingecko api).

Le dictionnaire doit sauvegarder toutes ses données(sauf le current price) dans un fichier wallet_name.dic
Au démmarage du programme toutes les entrées sont chargé en mémoire.
Chaque nouveau token, est sauvegardé dans le fichier.

Le module dictionnaire doit :
 - ✓ Permettre l’ajout d’un token.
 -   Valider l’ajout d’un token (avec validation de l’api coingecko).
 -   Fournir à partire d’un ID_Key, le token.
 - ✓ Mise à jours en directe du prix.

#### coingecko api 
Ce module est le lien entre les données locales et les valeurs sur internet.
Il doit permettre :
  - ✓ La mise à jours des prix d’un dictionnaire.
  - La recherche / validation de nouveaux tokens :
    - ✓ Grep sed to implement in c : `cat tist.txt | sed -E 's/.\{([^\}]*)\}/\1\n/g' | grep '"symbol":"btc"'`


#### main v1.0 
Le main est un CLI qui doit fournir les commandes suivante :
  - Multiple main pour faire un CLI :
    - Le token_search peut être un programme a part entière :
        - Télécharge et met en forme la liste des token disponible chez coingecko.
        - Enregistre la liste sous format csv avec un timestamp.
        - Si le timestamp est suppérieur à 1J, re-télécharge la liste, supprime l’ancien fichier et en créer un nouveau.
        - Effectue les recherches sur se fichier.
    - token search [-i ID] [-s symbol] [-n name] => retourne une liste des token disponibles :
       - num (position dans le csv), id, symbol, name\n
    - token add num => Ajoute au dictionnaire le token de la position num dans le fichier CSV.
    - token update (update the file)

  - wallet :
    - Affichage des toteaux du wallet par défaut.
    - Affichage de la valeur FIAT totale + les gains perte.
    - Pour chaque token en possession affichage du nombre, de la valeur, et des gain/perte.
  - wallet [token_name | token_id | token_short] affichage des possessions de ce token :
      - Affichage des toteaux (possession, valeur, perte gain).
      - Affichage de la liste des transaction par date plus récente au plus ancien.
  - wallet add trans [date] [token] [amount] [value] :
       - ajoute une transaction en vérifiant les paramètres, une suite de menu peut guider l’utilisateur.
  - wallet add token [name] :
        - Guide l’utilateur pour rechercher et ajouter un nouveau token au dictionnaire.
