/***************************************************************************/
/*                                                                         */
/* 234218 Data DSs 1, Winter 2015-2016                                     */
/*                                                                         */
/* Homework : Wet 1                                                        */
/*                                                                         */
/***************************************************************************/

/***************************************************************************/
/*                                                                         */
/* File Name : main1.cpp                                                   */
/*                                                                         */
/* Holds the "int main()" function and the parser of the shell's           */
/* command line.                                                           */
/***************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Statistics.h"
#include <iostream>
using namespace std;


void print(Statistics& st)
{
	int *a, len;
	assert(st.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	for (int i = 0; i < len; ++i) {
		printf("Pokemon %d   :   ID = %d\n", i, a[i]);
	}
	printf("END\n");
	free(a);
	a=0;
}

void printTop(Statistics& st, int id)
{
	int top;
	assert(st.GetTopPokemon(id, &top) == SUCCESS);
	printf("top pokimon of trainer %d is   :   %d\n", id, top);
}

int main() {

	printf("MY TEST:\n\n");

	Statistics st;

	// AddTrainer
	assert(st.AddTrainer(0) == INVALID_INPUT);
	assert(st.AddTrainer(-650) == INVALID_INPUT);
	assert(st.AddTrainer(315312) == SUCCESS);

	assert(st.AddTrainer(0) == INVALID_INPUT);
	assert(st.AddTrainer(-650) == INVALID_INPUT);
	assert(st.AddTrainer(315312) == FAILURE);
	assert(st.AddTrainer(315312) == FAILURE);
	assert(st.AddTrainer(315311) == SUCCESS);
	assert(st.AddTrainer(315311) == FAILURE);

	for (int i = 40; i < 50; ++i) {
		assert(st.AddTrainer(i) == SUCCESS);
	}


	//CatchPokemon
	assert(st.CatchPokemon(-93, 47, 98) == INVALID_INPUT);
	assert(st.CatchPokemon(0, 47, 98) == INVALID_INPUT);
	assert(st.CatchPokemon(-93, 65, 0) == INVALID_INPUT);
	assert(st.CatchPokemon(93, 47, 0) == INVALID_INPUT);
	assert(st.CatchPokemon(5, 47, -7) == INVALID_INPUT);
	assert(st.CatchPokemon(3, -47, 98) == INVALID_INPUT);
	assert(st.CatchPokemon(59, 47, 0) == INVALID_INPUT);

	assert(st.CatchPokemon(93, 47, 98) == SUCCESS);
	assert(st.CatchPokemon(93, 47, 98) == FAILURE);
	assert(st.CatchPokemon(93, 315311, 98) == FAILURE);
	assert(st.CatchPokemon(46, 47, 98) == SUCCESS);
	assert(st.CatchPokemon(19, 47, 1) == SUCCESS);
	assert(st.CatchPokemon(94, 315311, 98) == SUCCESS);
	assert(st.CatchPokemon(35, 315211, 98) == FAILURE);

	assert(st.CatchPokemon(93, 0, 98) == INVALID_INPUT);
	assert(st.CatchPokemon(-93, 68, 98) == INVALID_INPUT);
	assert(st.CatchPokemon(93, 42, -1) == INVALID_INPUT);

	for (int i = 0; i < 10; ++i) {
		assert(st.CatchPokemon(i+1, 40+i, 65-i) == SUCCESS);
		assert(st.CatchPokemon(i+1111, 49-i, 52+i) == SUCCESS);
	}

	for (int i = 0; i < 10; ++i) {
		assert(st.CatchPokemon(i+1, 49-i, 7+8*i) == FAILURE);
		assert(st.CatchPokemon(i+1111, 40+i, 79+4*i) == FAILURE);
	}


	// FreePokemon:

	assert(st.CatchPokemon(8, 315311, 98) == FAILURE);
	assert(st.FreePokemon(8) == SUCCESS);
	assert(st.FreePokemon(8) == FAILURE);
	assert(st.CatchPokemon(8, 315311, 58) == SUCCESS);

	print(st);

	for(int i=0; i<10; i++){
		assert(st.FreePokemon(-i) == INVALID_INPUT);
		assert(st.FreePokemon(i+1111) == SUCCESS);
	}

	print(st);

	for (int i = 0; i < 10; ++i) {
		assert(st.CatchPokemon(i+1, 40+i, 65-i) == FAILURE);
		assert(st.CatchPokemon(i+1111, 49-i, 500) == SUCCESS);
	}
	print(st);
	for (int i = 40; i < 50; ++i) {
		printTop(st, i);
	}

	// LevelUp

	int *a, len;
	assert(st.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	for (int i = 0; i < len; ++i) {
		if(a[i] > 1000) continue;

		assert(st.LevelUp(a[i],1)== SUCCESS);
		assert(st.LevelUp(-a[i],1) == INVALID_INPUT);
		assert(st.LevelUp(0,i) == INVALID_INPUT);
		assert(st.LevelUp(a[i],0) == INVALID_INPUT);
		assert(st.LevelUp(a[i],-i) == INVALID_INPUT);
		assert(st.LevelUp(a[i],546) == SUCCESS);
		assert(st.LevelUp(1000+1,546) == FAILURE);

	}
	free(a);
	a=0;

	print(st);

	for (int i = 40; i < 50; ++i) {
		printTop(st, i);
	}

	int top;
	assert(st.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 46);
	assert(st.LevelUp(6, 1000) == SUCCESS);
	assert(st.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 6);


	// GetTopPokemon, GetAllPokemonsByLevel
	Statistics st2;

	assert(st.CatchPokemon(777, 315311, 7777) == SUCCESS);
	assert(st.GetTopPokemon(315311, &top) == SUCCESS);
	assert(top == 777);
	assert(st.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 777);
	assert(st.CatchPokemon(666, 315311, 7777) == SUCCESS);
	assert(st.GetTopPokemon(315311, &top) == SUCCESS);
	assert(top == 666);
	assert(st.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 666);
	assert(st.AddTrainer(555) == SUCCESS);
	assert(st.GetTopPokemon(555, &top) == SUCCESS);
	assert(top == -1);
	assert(st.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 666);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == -1);
	assert(st.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(a!=0 && len!=0);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(a==0&&len==0);
	assert(st2.AddTrainer(444) == SUCCESS);
	assert(st.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(a!=0 && len!=0);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(a==0&&len==0);
	assert(st2.GetTopPokemon(444, &top) == SUCCESS);
	assert(top == -1);
	assert(st2.CatchPokemon(1, 444, 1) == SUCCESS);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(*a==1&&len==1);
	assert(st2.GetTopPokemon(444, &top) == SUCCESS);
	assert(top == 1);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 1);
	assert(st2.AddTrainer(231) == SUCCESS);
	assert(st2.AddTrainer(654) == SUCCESS);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(*a==1&&len==1);
	assert(st2.GetAllPokemonsByLevel(231, &a, &len) == SUCCESS);
	assert(a==0 && len==0);
	assert(st2.GetTopPokemon(231, &top) == SUCCESS);
	assert(top == -1);
	assert(st2.GetTopPokemon(444, &top) == SUCCESS);
	assert(top == 1);
	assert(st2.GetTopPokemon(654, &top) == SUCCESS);
	assert(top == -1);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 1);
	assert(st2.CatchPokemon(2, 231, 2) == SUCCESS);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 2);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(a[0]==2&&a[1]==1&&len==2);
	assert(st2.GetTopPokemon(0, &top) == INVALID_INPUT);
	assert(st2.GetTopPokemon(1, NULL) == INVALID_INPUT);
	assert(st2.GetTopPokemon(1, &top) == FAILURE);
	assert(st2.GetAllPokemonsByLevel(-1, 0, &len) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(-1, &a, 0) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(444, 0, &len) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(444, &a, 0) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(584, 0, &len) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(584, &a, 0) == INVALID_INPUT);
	assert(st2.GetAllPokemonsByLevel(584, &a, &len) == FAILURE);


	//EvolvePokemon

	assert(st2.CatchPokemon(3, 231, 2) == SUCCESS);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 2);
	assert(st2.EvolvePokemon(2, 15) == SUCCESS);
	assert(st2.GetTopPokemon(-1, &top) == SUCCESS);
	assert(top == 3);
	assert(st2.GetAllPokemonsByLevel(-1, &a, &len) == SUCCESS);
	assert(len==3&&a[0]==3&&a[1]==15&&a[2]==1);

	assert(st2.EvolvePokemon(-15, 64) == INVALID_INPUT);
	assert(st2.EvolvePokemon(15, -64) == INVALID_INPUT);
	assert(st2.EvolvePokemon(6, 0) == INVALID_INPUT);
	assert(st2.EvolvePokemon(0, 1) == INVALID_INPUT);
	assert(st2.EvolvePokemon(15, 1) == FAILURE);
	assert(st2.EvolvePokemon(6, 64) == FAILURE);
	assert(st2.EvolvePokemon(15, 64) == SUCCESS);


	//UpdateLevels

	for (int i = 1; i <= 7; ++i) {
		assert(st2.AddTrainer(i) == SUCCESS);
		assert(st2.CatchPokemon(i+21,i,i+2) == SUCCESS);
	}
	assert(st2.AddTrainer(13) == SUCCESS);

	assert(st2.CatchPokemon(888,3,5) == SUCCESS);
	assert(st2.CatchPokemon(1000,3,7) == SUCCESS);
	assert(st2.CatchPokemon(865,4,8) == SUCCESS);
	assert(st2.CatchPokemon(101,4,7) == SUCCESS);
	assert(st2.CatchPokemon(19,5,999) == SUCCESS);
	assert(st2.CatchPokemon(97,6,1) == SUCCESS);
	assert(st2.CatchPokemon(21,7,20) == SUCCESS);

	assert(st2.UpdateLevels(15555, 7) == SUCCESS);
	print(st2);

	assert(st2.UpdateLevels(1, 7) == SUCCESS);
	print(st2);

	assert(st2.UpdateLevels(2, 3) == SUCCESS);
	print(st2);

	assert(st2.UpdateLevels(3, 1) == SUCCESS);
	print(st2);

	for (int i = 1; i <= 7; ++i) {
		printTop(st2, i);
	}

	printf("\nSUCCESS");
	return 0;
}

