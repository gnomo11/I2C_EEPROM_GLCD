//	P3.2					** COMUNICACIÓN I2C **
//---------------------------------------------------------------------------
#include <18F4550.h>
#device CONST=READ_ONLY
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NOBROWNOUT,NODEBUG,USBDIV,PLL2,CPUDIV1,VREGEN
#use delay(clock=48000000)
#use i2c(master, scl=PIN_B1, sda=PIN_B0, fast=300000, FORCE_HW)
#include <st7920_D.h>
#include <graphics.c>
#include <kbd1.c>
#include <stdlib.h>
//#include <DS3231_1.c> 
//---------------------------------------------------------------------------
void menu_principal(void);
void lectura(void);
void escritura(void);
void direccionamiento(void);
void direccionamiento2(void);
void escribir_dato(void);
void animacion(void);
void EEPROM_SAVE(long direc_write, int dat);
int EEPROM_read (long direc_read);
//---------------------------------------------------------------------------



//----------------------------------------------------------------------------
char mens[50],address[4],data[4],rdi[5],tecla,opd[5];
int i=0,opcion=0,cx=16,j=0,q=0;
long direccion,dato;
//---------------------------------------------------------------------------
void main(void) {
	glcd_init_graph();
	kbd_init();
	
	glcd_rect(0,0,127,63,NO,ON);		//Diseño de la Interfaz
	glcd_update();
	for(;;){
		
		menu_principal();

		if(opcion==1){lectura();}
		else if(opcion==2){escritura();}
	
	}
}
//---------------------------------------------------------------------------
void menu_principal(void){

	sprintf(mens,"Seleccionar opcion:");
	glcd_rect(6,5,120,16,YES,ON);
	glcd_text57(8,7,&mens,1,OFF);

	sprintf(mens,"A) LECTURA");
	glcd_text57(25,25,&mens,1,ON);
	glcd_rect(6,21,18,35,YES,ON);
	glcd_circle(12,24,2,YES,OFF);
	glcd_line(11,28,8,25,OFF);
	glcd_line(11,33,8,30,OFF);
	glcd_line(8,26,8,29,OFF);
	glcd_line(13,28,16,25,OFF);
	glcd_line(13,33,16,30,OFF);
	glcd_line(16,26,16,29,OFF);
	glcd_line(12,28,12,33,OFF);

	sprintf(mens,"B) ESCRITURA");
	glcd_text57(25,44,&mens,1,ON);
	glcd_rect(6,40,18,54,YES,ON);
	glcd_rect(8,41,16,53,NO,OFF);
	glcd_line(10,43,14,43,OFF);
	glcd_line(10,45,14,45,OFF);
	glcd_line(10,47,14,47,OFF);
	glcd_line(10,49,14,49,OFF);
	glcd_line(10,51,12,51,OFF);
	glcd_update();
	
ret:while(tecla==0) {
		tecla = kbd_getc();
	}

	if(tecla=='A'){
		opcion=1;
		glcd_rect(23,23,84,33,YES,OFF);
		sprintf(mens,"A) LECTURA");
		glcd_rect(23,23,84,33,YES,ON);
		glcd_text57(25,25,&mens,1,OFF);
		glcd_update();
		delay_ms(400);
	}
	else if(tecla=='B'){
		opcion=2;
		sprintf(mens,"B) ESCRITURA");
		glcd_rect(23,42,97,53,YES,OFF);
		glcd_update();
		glcd_rect(23,42,97,53,YES,ON);
		glcd_text57(25,44,&mens,1,OFF);
		glcd_update();
		delay_ms(400);
	}
	else{
		opcion=0;
		tecla=0;
		goto ret;
	}

	glcd_rect(4,5,123,58,YES,OFF);
	glcd_update();
	tecla=0;
}
//---------------------------------------------------------------------------
void lectura(void){
	sprintf(mens,"Direccion:");
	glcd_text57(10,12,&mens,1,ON);
	glcd_rect(13,24,41,34,NO,ON);
	glcd_rect(85,26,122,60,NO,ON);
	sprintf(mens,"#->");
	glcd_text57(90,50,&mens,1,ON);
	glcd_circle(115,53,4,YES,ON);
	glcd_line(117,51,114,54,OFF);
	glcd_pixel(113,53,OFF);
	sprintf(mens,"D->");
	glcd_text57(90,40,&mens,1,ON);
	glcd_circle(115,43,4,YES,ON);
	glcd_line(115,43,118,43,OFF);
	glcd_line(114,41,114,45,OFF);
	glcd_line(113,42,113,44,OFF);
	glcd_pixel(112,43,OFF);
	sprintf(mens,"C->");
	glcd_text57(90,30,&mens,1,ON);
	glcd_circle(115,33,4,YES,ON);
	glcd_line(117,31,113,35,OFF);
	glcd_line(117,35,113,31,OFF);
	
	glcd_update();

	direccionamiento();
	tecla=0;i=0;cx=16;
	delay_ms(400);
	

	glcd_rect(4,5,123,62,YES,OFF);
	
}
//---------------------------------------------------------------------------
void escritura(void){

	sprintf(mens,"Direccion:");
	glcd_text57(10,12,&mens,1,ON);
	glcd_rect(13,24,41,34,NO,ON);

	sprintf(mens,"Dato:");
	glcd_text57(10,38,&mens,1,ON);
	glcd_rect(13,50,35,60,NO,ON);
	glcd_rect(85,26,122,60,NO,ON);
	sprintf(mens,"#->");
	glcd_text57(90,50,&mens,1,ON);
	glcd_circle(115,53,4,YES,ON);
	glcd_line(117,51,114,54,OFF);
	glcd_pixel(113,53,OFF);
	sprintf(mens,"D->");
	glcd_text57(90,40,&mens,1,ON);
	glcd_circle(115,43,4,YES,ON);
	glcd_line(115,43,118,43,OFF);
	glcd_line(114,41,114,45,OFF);
	glcd_line(113,42,113,44,OFF);
	glcd_pixel(112,43,OFF);
	sprintf(mens,"C->");
	glcd_text57(90,30,&mens,1,ON);
	glcd_circle(115,33,4,YES,ON);
	glcd_line(117,31,113,35,OFF);
	glcd_line(117,35,113,31,OFF);
	
	
	glcd_update();

 back4: direccionamiento2();
		i=0;cx=16;
	
	if(tecla!='D'){
	tecla=0;
	escribir_dato();
		if(tecla=='D'){
		tecla=0;i=0;cx=16;j=0;
		glcd_rect(14,51,34,59,YES,OFF);//clear dato
		glcd_circle(51,55,4,YES,OFF);
		glcd_rect(14,25,40,33,YES,OFF);	//clear direcc
		glcd_circle(51,29,4,YES,OFF);
		glcd_update();
		goto back4;
		}
	tecla=0;j=0;cx=16;
	}

	delay_ms(400);
	glcd_rect(4,5,123,62,YES,OFF);

}
//---------------------------------------------------------------------------
void direccionamiento(void){
	int dx;	

 
back:	while(tecla!='#'&&tecla!='D'){
		tecla = kbd_getc();

		if(tecla>='0' && tecla<='9'&&i<4){
			address[i]=tecla;
			sprintf(opd,"%c",tecla);
			glcd_text57(cx,26,&opd,1,ON);
			glcd_update();
			cx=cx+6;
			i++;
		}

		else if(tecla=='C'&&i>=1){
			cx=cx-6;
			i--;
			address[i]=" ";
			glcd_rect(cx,26,cx+5,32,YES,OFF);
			glcd_update();
		}
	}

	if(tecla=='#'){
	direccion=atoi32(address);
	for(q=0;q<4;q++){address[q]=" ";}

		if(direccion>4095){
			glcd_circle(51,29,4,YES,ON);
			glcd_line(53,27,49,31,OFF);
			glcd_line(53,31,49,27,OFF);	
		}
		else{
			glcd_circle(51,29,4,YES,ON);
			glcd_line(53,27,50,30,OFF);
			glcd_pixel(49,29,OFF);
			sprintf(rdi,"El dato es:");
			glcd_text57(5,40,&rdi,1,ON);
			
		//------------------------------------
			dx = EEPROM_read (direccion);
		//------------------------------------
		
			delay_ms(10);
			sprintf(rdi,"%u",dx);
			glcd_text57(25,50,&rdi,1,ON);
		}
		glcd_update();

	while(tecla!='D'){tecla = kbd_getc();}
			tecla=0;i=0;cx=16;
			glcd_rect(4,39,70,62,YES,OFF);//clear el dato es...
			glcd_rect(14,25,40,33,YES,OFF);	//clear direcc
			glcd_circle(51,29,4,YES,OFF);
		
		glcd_update();
		goto back;
	}
	
}
//---------------------------------------------------------------------------
void direccionamiento2(void){
	
back2:	while(tecla!='#'&&tecla!='D'){
		tecla = kbd_getc();

		if(tecla>='0' && tecla<='9'&&i<4){
			address[i]=tecla;
			sprintf(opd,"%c",tecla);
			glcd_text57(cx,26,&opd,1,ON);
			glcd_update();
			cx=cx+6;
			i++;
		}

		else if(tecla=='C'&&i>=1){
			cx=cx-6;
			i--;
			address[i]=" ";
			glcd_rect(cx,26,cx+5,32,YES,OFF);
			glcd_update();
		}
	}

	if(tecla=='#'){
	direccion=atoi32(address);
	for(q=0;q<4;q++){address[q]=" ";}

		if(direccion>4095){
			glcd_circle(51,29,4,YES,ON);
			glcd_line(53,27,49,31,OFF);
			glcd_line(53,31,49,27,OFF);
			glcd_update();	
			while(tecla!='D'){tecla = kbd_getc();}
			tecla=0;i=0;cx=16;
			glcd_rect(14,25,40,33,YES,OFF);	//clear direcc
			glcd_circle(51,29,4,YES,OFF);
			glcd_update();
			goto back2;
		}

		else{
			glcd_circle(51,29,4,YES,ON);
			glcd_line(53,27,50,30,OFF);
			glcd_pixel(49,29,OFF);
		}
		glcd_update();

	}
	
}

//---------------------------------------------------------------------------
void escribir_dato(void){
	
back3:	while(tecla!='#'&&tecla!='D'){
		tecla = kbd_getc();
		
		if(tecla>='0' && tecla<='9'&&j<=2){
		data[j]=tecla;
		sprintf(opd,"%c",tecla);
		glcd_text57(cx,52,&opd,1,ON);
		glcd_update();
		cx=cx+6;
		j++;
		}

		else if(tecla=='C'&&j>=1){
			cx=cx-6;
			j--;
			data[j]=" ";
			glcd_rect(cx,52,cx+5,58,YES,OFF);
			glcd_update();
		}
	}
	

	if(tecla=='#'){
	dato=atoi32(data);
	for(q=0;q<3;q++){data[q]=" ";}

	if(dato>255){
		glcd_circle(51,55,4,YES,ON);
		glcd_line(53,53,49,57,OFF);
		glcd_line(53,57,49,53,OFF);
		glcd_update();
		while(tecla!='D'){tecla = kbd_getc();}
		tecla=0;j=0;cx=16;
		glcd_rect(14,51,34,59,YES,OFF);//clear dato
		glcd_circle(51,55,4,YES,OFF);
		glcd_update();
		goto back3;
	}
	else {
		glcd_circle(51,55,4,YES,ON);
		glcd_line(53,53,50,56,OFF);
		glcd_pixel(49,55,OFF);
		glcd_update();
	}
//--------------------------------
	
	EEPROM_SAVE(direccion,dato);
		//delay_us(10);
//--------------------------------
	delay_ms(300);
	animacion();
		
	}


	
}
//------------------------------------------------------
void animacion (void){
	int a=31;

	glcd_rect(1,1,126,62,YES,OFF);
	glcd_rect(20,34,113,42,NO,ON);
	glcd_update();

	while(a<=120){
	sprintf(mens,"ESCRIBIENDO...");
	glcd_text57(25,10,&mens,1,ON);
	glcd_rect(22,36,a,40,YES,ON);
	glcd_update();
	delay_ms(200);
	a=a+10;
	}
	a=51;	
} 


//-----------------------------------------------------
//-----------------------------------------------------

void EEPROM_SAVE(long direc_write, int dat){
	i2c_start();
	i2c_write(0xAE);
	i2c_write(direc_write >> 8); //enviar la parte ALTA
	i2c_write(direc_write); //enviar la parte BAJA
	i2c_write(dat);
	i2c_stop();
	delay_us(10);
}

//-------------------------------------------------------------

int EEPROM_read (long direc_read){
	int data_return;

	i2c_start();
	i2c_write (0xAE); 
	i2c_write(direc_read >> 8);
	i2c_write(direc_read);
	i2c_start();
	i2c_write(0xAF);
	data_return=i2c_read(0);
	i2c_stop();
	delay_us(10);
	return (data_return);
}
