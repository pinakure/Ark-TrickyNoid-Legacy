#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H

//Enviromental class
typedef struct s_envunit* ENVUNIT;
typedef struct s_envunit
{
	void (*init		)(void);	//Init enviromental system process
	void (*deinit  )(void);	//Deinit environmental system process
	void (*fade		)(void);	//Fade backbuffer process
	void (*blend	)(void);	//Blend backbuffer process
	void (*clear	)(void);	//Clear backbuffer process
	void (*tick		)(void);	//Enviromental system tick process
	int bias_fade;
	int bias_blend;
	int freq_fade;
	int freq_blend;
	int wind; //if != 0, value is the speed of the wind.
}envunit;

#endif