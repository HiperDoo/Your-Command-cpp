# Your-Command-cpp

Este proyecto tiene como finalidad ejecutar funciones con comandos ingresados por el usuario a tiempo de ejecucion...

El programa cuenta con 8 comandos de prueba (con capacidad de agregar cuantos comandos quieras para implementarlo en tu proyecto):
  hello <= Imprime un hello world
  
  say "text" <= Imprime la cadena de texto que hayas escrito en "text"
  
  say2 #num ["text"] <= Imprime un numero y una cadena de texto (este texto debe estar encerrado entre [])
  
  say3 #num ["text"] #num <= Imprime dos numeros y una cadena de texto
  
  say4 #num ["text"] ["text"] #num <= Imprime dos numeros y dos cadenas de texto
  
  sum #num #num #num <= Imprime la sumatoria de 3 numeros
  
  res #num #num <= Imprime la resta de 2 numeros
  
  exit <= Cierra el programa
  
  
  Ejemplos:
  
  say Hola a todos!
    > User: Hola a todos!
    
  say3 2020 [Hola a todos!] 2021
    > User2020: Hola a todos num: 2021
    
  sum 300 20 1
    > Console: 300 + 20 + 1 = 321
