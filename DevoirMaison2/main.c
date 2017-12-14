// UE LAngage C - DM 2- 2017
// Numero étudiant : 37003846
// Nom      :        Budulla
// Prenom   :        Akhlaaq Oumair Mian
// Filiere  :        L2 Informatique

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	char nom[30];			// Nom de la station
	double longitude;		// Coordonnes GPS
	double latitude;		// Coordonnes GPS
	int altitude;			// Altitude
	int SO2[24];			// Mesures du monoxyde d’azote en mg/m3
	int PM10[24];			// Mesures des particules PM10 en mg/m3
	int hmaxSO2;			// Heure ou le SO2 est max
	int hmaxPM10;			// Heure ou le PM10 est max
} STATION;

//function
STATION lireUneMesure(FILE * fich) {
	STATION st;
	//nom
	fgets(st.nom, 30, fich);
	st.nom[strlen(st.nom) - 1] = ' ';

	//longitude
	char ligne[20];
	int i = 0;
	char c = fgetc(fich);
	while (c != '\n') {
		ligne[i] = c;
		i++;
		c = fgetc(fich);
	}
	st.longitude = strtod(ligne, NULL);

	//latitude
	i = 0;
	c = fgetc(fich);
	while (c != '\n') {
		ligne[i] = c;
		i++;
		c = fgetc(fich);
	}
	st.latitude = strtod(ligne, NULL);

	//altitude
	i = 0;
	c = fgetc(fich);
	while (c != '\n') {
		ligne[i] = c;
		i++;
		c = fgetc(fich);
	}
	st.altitude = strtod(ligne, NULL);

	//SO2
	int j = 0;
	//i = 0;
	for (j = 0; j < 24; j++) {
		fscanf(fich, "%d", &st.SO2[j]);
	}

	//PM10
	i = 0;
	for (j = 0; j < 24; j++) {
		fscanf(fich, "%d", &st.PM10[j]);
	}

    //hmaxSO2
	int n = st.SO2[0];
	for(i=0;i<24;i++){
        if(n<st.SO2[i]){
            n=st.SO2[i];
            st.hmaxSO2=i+1;
        }
	}

	int mn = st.PM10[0];
	for(i=0;i<24;i++){
        if(mn<st.PM10[i]){
            mn=st.PM10[i];
            st.hmaxPM10=i+1;
        }
	}

	return st;
}

//function
void afficheData(STATION stat) {//3tab
	printf("      ****** %s ****** \n", stat.nom);
	printf("Longitude                               : %lf \n",stat.longitude);
	printf("latitude                                : %lf \n",stat.latitude);
	printf("altitude                                : %d \n",stat.altitude);
	printf("Taux de monoxyde d'azote (mg/m3): \n");
	int i=0;
	for(i=0;i<24;i++){
        printf("%dH  :  %d \n",i+1,stat.SO2[i]);
	}
	printf("\n");
	printf("Taux de PM10 (mg/m3): \n");
	for(i=0;i<24;i++){
        printf("%dH  :  %d \n",i+1,stat.PM10[i]);
	}
	printf("\n");
	printf("Heur max SO2                            : %d \n",stat.hmaxSO2);
	printf("Heur max PM10                           : %d \n\n\n",stat.hmaxPM10);
}


//function
void genereCSV(STATION *tabstat, char *fichier) {
FILE*fich;
fich = fopen(fichier,"w+");
fputs("Station, Longitude, Latitude, Altitude,", fich);
int i=0,j=0,k=0;

for(i=1;i<25;i++){
    char nom_col[20];
    sprintf(nom_col,"SO2-%dH,",i);
    fputs(nom_col,fich);
}

for(i=1;i<25;i++){
    char nom_col[20];
    sprintf(nom_col,"PM10-%dh,",i);
    fputs(nom_col,fich);
}
fputs("HMAX-SO2,HMAX-PM10\n",fich);

for(i=0;i<8;i++){
  fprintf(fich,"%s, %f, %f, %d,",tabstat[i].nom,tabstat[i].longitude,tabstat[i].latitude,tabstat[i].altitude);
    for(j=0;j<24;j++){
    fprintf(fich,"%d,",tabstat[i].SO2[j]);
    }

        for(k=0;k<24;k++){
        fprintf(fich,"%d,",tabstat[i].PM10[k]);
        }
        fprintf(fich, "%d,%d,",tabstat[i].hmaxSO2, tabstat[i].hmaxPM10);
        fputs("\n",fich);
    }
    fclose(fich);

}



int main(void) {
	FILE *fichier;
	STATION tab_station[8];

	char nom_fichier[20];
	int i = 1;
	for(i=1;i<9;i++){
        sprintf(nom_fichier,"data0%i.txt",i);
        fichier = fopen(nom_fichier,"r+");
        tab_station[i-1] = lireUneMesure(fichier);
        afficheData(tab_station[i-1]);
        printf("\n");
        fclose(fichier);
	}
	genereCSV(tab_station,"devoir.csv");
	return 0;
}
