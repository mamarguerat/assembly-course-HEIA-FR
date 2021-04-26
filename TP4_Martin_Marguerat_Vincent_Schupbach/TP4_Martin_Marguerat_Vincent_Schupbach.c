/*****************************************************************************
  * @file    TP4_Martin_Marguerat_Vincent_Schupbach.c
  * @author  MARGUERAT Martin et SCHUPBACH Vincent
  * @date    22.04.2021
  * @brief   Donnée de l'exercice dans le cahier des charges du TP4
  *             Détermination du maximum, minimum, moyenne et tri d'un tableau
  *
  *
  ****************************************************************************/

#include <msp430.h>
#include <stdio.h>
#include <stdbool.h>

//----------------------------------------------------------------------------
// local definitions
//----------------------------------------------------------------------------
#define NBR_VAL 15
//----------------------------------------------------------------------------
// local functions declaration
//----------------------------------------------------------------------------
unsigned char trouveMin(unsigned char arrayIn[]);
unsigned char trouveMax(unsigned char arrayIn[]);
unsigned char calculMoyenne(unsigned char arrayIn[]);
void clignotteLED(unsigned char nbrClignottement);
void bubbleSort(unsigned char arrayIn[]);
unsigned int sommeVal(unsigned char arrayIn[]);
//----------------------------------------------------------------------------
// main function
//----------------------------------------------------------------------------
int main( void )
{
  WDTCTL = WDTPW + WDTHOLD;// Stop watchdog timer to prevent time out reset
  PM5CTL0 &= ~LOCKLPM5;    // Disable the GPIO power-on default high-impedance mode
                           // to activate previously configured port settings

  P1OUT &= ~BIT0;
  P1DIR |= BIT0;            //Mettre la LED sur le port P1.0 en sortie

  //Déclaration et initialisation des variables
  unsigned char cArray[NBR_VAL] = {4, 9, 0, 1, 5, 8, 10, 7, 6, 2, 3, 1, 4, 2, 6};
  unsigned char min = 0;
  unsigned char max = 0;
  unsigned char moyenne = 0;

  //Appel des fonctions pour notre programme
  min = trouveMin(cArray);
  max = trouveMax(cArray);
  moyenne = calculMoyenne(cArray);
  clignotteLED(moyenne);
  bubbleSort(cArray);

  while (1);    //Boucle sans fin
}
//----------------------------------------------------------------------------
// local functions
//----------------------------------------------------------------------------
/**
 * Fonction cherchant la valeur la plus petite d'un tableau
 * @param arrayIn   tableau de NBR_VAL valeurs
 * @return  valeur min trouvée dans le tableau
 */
unsigned char trouveMin(unsigned char arrayIn[])
{
    unsigned char minVal = arrayIn[0];
    unsigned int i;
    for(i = 1; i < NBR_VAL; i++)
    {
        if(arrayIn[i] < minVal) //Si le nombre est plus grand que le nombre enregistré, enregistrer le nouveau nombre
        {
            minVal = arrayIn[i];
        }
    }
    return minVal;
}

/**
 * Fonction cherchant la valeur la plus grande d'un tableau
 * @param arrayIn   tableau de NBR_VAL valeurs
 * @return  valeur max trouvée dans le tableau
 */
unsigned char trouveMax(unsigned char arrayIn[])
{
    unsigned char maxVal = arrayIn[0];
    unsigned int i;
    for(i = 1; i < NBR_VAL; i++)
    {
        if(arrayIn[i] > maxVal) //Si le nombre est plus grand que le nombre enregistré, enregistrer le nouveau nombre
        {
            maxVal = arrayIn[i];
        }
    }
    return maxVal;
}

/**
 * Fonction calculant la moyenne des nombre dans un tableau
 * @param arrayIn   tableau de NBR_VAL valeurs
 * @return  Moyenne calculée arrondie à l'entier en dessous
 */
unsigned char calculMoyenne(unsigned char arrayIn[])
{
    unsigned char moyenne = 0;
    unsigned int somme = 0;
    somme = sommeVal(arrayIn);
    moyenne = somme/NBR_VAL;    //Diviser la somme des valeurs par le nombre de valeurs -> moyenne
    return moyenne;
}

/**
 * Fonction faisait clignotter la led à une fréquence de 2Hz pour un nombre de fois défini
 * @param nbrClignottement  nombre de fois que la LED doit s'allumer
 */
void clignotteLED(unsigned char nbrClignottement)
{
    unsigned int i;
    for(i = 0; i < nbrClignottement; i++)
    {
        P1OUT |= BIT0;  //Allumer la LED
        __delay_cycles(250000);
        P1OUT &= ~BIT0; //Eteindre la LED
        __delay_cycles(250000);
    }
}

/**
 * Fonction permettant de trier un tableau grâce à l'algorithme du Bubble Sort
 * (https://www.youtube.com/watch?v=lyZQPjUT5B4 et https://www.youtube.com/watch?v=xli_FI7CuzA)
 * @param arrayIn   tableau de NBR_VAL valeurs
 */
void bubbleSort(unsigned char arrayIn[])
{
    //L'algorithme est tiré du pseudo-code montré dans la vidéo https://www.youtube.com/watch?v=xli_FI7CuzA
    unsigned int i;
    for(i = 1; i < NBR_VAL; i++)
    {
        unsigned int j;
        for(j = 0; j < NBR_VAL - 1; j++)
        {
            if(arrayIn[j] > arrayIn[j+1])
            {
                unsigned char a = arrayIn[j];
                arrayIn[j] = arrayIn[j+1];
                arrayIn[j+1] = a;
            }
        }
    }
}

/**
 * Fonction calculant la somme de toutes les valeurs d'un tableau
 * @param arrayIn   tableau de NBR_VAL valeurs
 * @return  somme des valeurs du tableau
 */
unsigned int sommeVal(unsigned char arrayIn[])
{
    unsigned int somme = 0;
    unsigned int i;
    for(i = 0; i < NBR_VAL; i++)    //Pour chaque valeur du tableau, ajouter la valeur à la somme
    {
        somme += arrayIn[i];
    }
    return somme;
}
