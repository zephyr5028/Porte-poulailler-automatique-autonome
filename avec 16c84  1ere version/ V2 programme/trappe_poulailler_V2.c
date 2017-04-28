// commande d'ouverture v2.1
// date 25.01.2011
// gestion de la ldr par ampli ops
// pour la trappe du poulailler
// avec servo moteur
// et ldr pour la lumière
// compiler cc5x
#pragma chip PIC16F84A
//config _CP_OFF & _WDTE_OFF & _PWRTE_ON & _FOSC1-FSOC0_HS
#pragma config |= 0x3ff2
#include "int16CXX.h"
int compteur2;
long compteur1;
int sens;
bit signal;
bit etat;
bit servo_sous_tension;
bit tant_que;
long compteurdelay;
int defaut;
long tempsdelay;

int av;
int ar;
int stop;


// interruption
#pragma origin 4
interrupt inter_50(void) // pour cde sevo
{
int_save_registers // + autres si necessaire (option, FSR...)
TMR0 = 233;
if (servo_sous_tension == 1) {
if (++compteur1==360) {
compteur1=0;
compteur2=0;
signal = 1;

}
else {
if (signal == 1) {
if (compteur2 == sens) {
signal = 0;
PORTB.1=0;
}
else{
PORTB.1=1;
compteur2++;
}
}
}
}
compteurdelay++; // pour delay
if (PORTB.6 == 1) { // led clignotante
PORTB.6 = 0;
}
else {
PORTB.6 = 1;
}
T0IF = 0;
int_restore_registers
}

// fonction delay
void delay_s (long tempsdelay)
{
compteurdelay = 0;
while (compteurdelay <= tempsdelay) {
}
}

// fonction sleep avec reveil sur front descendant
void sleepfrontdescendant (void)
{
INTCON = 0b00010000; // bit INTE à 1 pour it sur front sur rb0
OPTION = 0b10000000; // bit INTEDG à 0 pour front descendant sur rb0
PORTB.6 = 0;
sleep();
INTCON = 0b10100000;
OPTION = 0b11000000;
}

// fonction ouverture
void ouverture (void)
{
if (PORTB.5 == 0) { // contacteur haut = 1 ouverture - trappe basse
PORTA.2 = 1; // cde relais
delay_s(10000);
sens = av;
servo_sous_tension = 1;
compteurdelay = 0;
defaut = 0;
while (PORTB.5 == 0 && defaut < 7)  {
if (compteurdelay == 20000) { // arrêt de securité
compteurdelay = 0;
defaut ++;
}
}
if (defaut == 7 ) {   // en mode automatique ou manuel
servo_sous_tension = 0;
PORTB.1 = 0;
PORTA.2 = 0;
T0IE = 0;
PORTB.6 = 0;
sleep();  // arrêt en mode sommeil (il faut une RAZ pour en sortir)
while (tant_que == 0) {
}
}
else {
sens = stop;
servo_sous_tension =0;
PORTA.2 = 0; // cde relais
}
}
}

// fonction fermeture
void fermeture (void)
{
if (PORTB.2 == 0) {  // contacteur bas  -  trappe haute
PORTA.2 = 1; // cde relais
delay_s(10000);
sens = ar;
servo_sous_tension = 1;
compteurdelay = 0;
defaut = 0;
while (PORTB.2 == 0 && defaut < 6) {
if (compteurdelay == 20000) {  // arrêt de sécurité
compteurdelay = 0;
defaut ++;
}
}
if (defaut == 6 && PORTB.4 == 0) {  // manuel
servo_sous_tension = 0;
PORTB.1 = 0;
PORTA.2 = 0;
T0IE = 0;
PORTB.6 = 0;
sleep();  // arrêt en mode sommeil (il faut une RAZ pour en sortir)
while (tant_que == 0) {
}
}
else {
sens = stop;
servo_sous_tension = 0;
PORTA.2 = 0; // cde relais
}
}
}

// programme
void main (void)
{
INTCON = 0b10100000; //bits GIE et T0IE à 1 pour utiliser l'interruption du timer
OPTION = 0b11000000;  // 
TRISB = 0b10111101;  // b6 et b1 en sortie
TRISA = 0b11111011;  // a2 en sortie
PORTB.6 = 1; // led
PORTA.2 = 0; // cde relais
PORTB.1 = 0; // cde servo moteur à 0
servo_sous_tension = 0;
signal = 1;  
etat = 1;
tant_que = 0;
av = 31;  // avant
ar = 21;  // retour
stop = 0;  // stop à 0
sens = stop;
compteur1 = 0;
compteur2 = 0;

// Fin des inits et debut du programme principal 

while (tant_que == 0) {

if (PORTB.4 == 0) { // manuel
delay_s(20000);
if (PORTB.4 == 0) {
if (PORTB.3 == 0) { // montée
delay_s(10000);
if (PORTB.3 == 0) {
ouverture();
}
delay_s(20000);
}
else {
if (PORTB.7 == 0) { // descente
delay_s(10000);
if (PORTB.7 == 0) {
fermeture();
}
delay_s(20000);
}
}
}
}

else {  // automatique
delay_s (20000);
if (PORTB.4 == 1 ) {  // automatique rb4 = 1
if (PORTB.5 == 1 ) {  // trappe haute rb5 = 1
if (PORTA.4 == 1 ) {  // ldr haut = 1, ra4 = 1
sleepfrontdescendant();
}
else {
if (PORTB.0 == 1 ) {  // ldr ou =1, rb0 = 1
sleepfrontdescendant();
}
else {
fermeture();
sleepfrontdescendant();
}
}
}
else {
if (PORTB.2 == 1 ) {  // trappe basse rb2 = 1
if (PORTA.4 == 0 ) {  // ldr haut = 0, ra4 = 0
sleepfrontdescendant();
}
else {
ouverture();
sleepfrontdescendant();
}
}
else {
if (PORTA.4 == 1 ) {  //  ldr haut = 1
ouverture();
sleepfrontdescendant();
}
else {
sleepfrontdescendant();
}
}
}
delay_s (20000);
}
}
}
}