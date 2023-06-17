# API Project 2022/2023

Algorithms and Data Structures Project

Grade: 30L

![C](https://img.shields.io/badge/C-A8B9CC?logo=C&logoColor=white&style=for-the-badge)

## Electric Vehicles Shortest Path
The objective of the project is as follows: given a couple of stations, plan the route with the least number of stages between them. If there are several routes with the same minimum number of stages, the route that prefers the stages with the shortest distance from the start of the highway must be chosen.

There are five different commands:
1) aggiungi-stazione distanza numero-auto autonomia-auto-1 ... autonomia-auto-n  
   Adds a station, located at the indicated distance, with a certain number of vehicles, with the given autonomy.  
   If there is already a station at that location, this station is not added.  
   Prints as an answer: 'aggiunta' (added) or 'non aggiunta' (not added).
2) demolisci-stazione distanza  
   Remove the station located at the indicated distance, if it exists.  
   Prints as an answer: 'demolita' (demolished) or 'non demolita' (not demolished).
3) aggiungi-auto distanza-stazione autonomia-auto-da-aggiungere  
   If the station exists, it adds a car to it. You can have multiple cars with the same autonomy.
   Prints as an answer: 'aggiunta' (added) or 'non aggiunta' (not added).
4) rottama-auto distanza-stazione autonomia-auto-da-rottamare  
   Remove a car from the indicated station, if the station exists and is equipped with at least one car with the indicated autonomy.
   Prints as an answer: 'rottamata' (scrapped) or 'non rottamata' (not scrapped).
5) pianifica-percorso distanza-stazione-partenza distanza-stazione-arrivo  
   It requires you to plan the route with the above constraints.
   Prints as an answer: the stations in order of journey, represented with the distance of the stations from the start 
   of the highway, separated by spaces and at the end followed by a new line.
   Departure and arrival must be included; if they coincide the station is printed only once. 
   If the route does not exist, print out any route. The planning action does not alter the stations or their vehicles. 
   The given stations are definitely present.

## Results
Constrains:

| Test's name | Time Limit      | Memory Limit |
|-------------|-----------------|--------------|
| Open        | 40,000 seconds  | 2,00 GiB     |
| 18          | 19,000 seconds  | 128 MiB      |
| 21          | 15,000 seconds  | 118 MiB      |
| 24          | T10,000 seconds | 108 MiB      |
| 27          | 6,000 seconds   | 98,0 MiB     |
| 30          | 4,000 seconds   | 88,0 MiB     |
| 30L         | 1,000 seconds   | 78,0 MiB     |

My results:

| Test's name | Result                | Execution time | Memory   |
|-------------|-----------------------|----------------|----------|
| Open        | The output is correct | 0,859 s        | 49,8 MiB |
| 18          | The output is correct | 0,886 s        | 49,9 MiB |
| 21          | The output is correct | 0,880 s        | 49,7 MiB |
| 24          | The output is correct | 0,882 s        | 49,8 MiB |
| 27          | The output is correct | 0,878 s        | 49,8 MiB |
| 30          | The output is correct | 0,884 s        | 49,8 MiB |
| 30L         | The output is correct | 0,860 s        | 49,8 MiB |




