#include "myLibs.hpp"

char* trovaPagina(char* s);

int main(int argc, char  *argv[]){
	Address* server;
	ClientTCP* myself;
	char* ip;

	char* pagina;
	char* ritorno;
	char* nomeFile;
	FILE* f;
	if(argc!=3){
		printf("USAGE:%s IP FILE\n",argv[0]);
		return -1;
	}

	ip = argv[1];
	nomeFile = argv[2];

	if(nomeFile == "/")	nomeFile = "index.html";

	server = new Address(ip,80);
	myself = new ClientTCP();

	bool riuscito = myself->connetti(*server);
	if (riuscito){
		printf("ERRORE CONNESSIONE\n");
		return -2;
	}
	printf("connesso\n");
	int lunghezza = strlen(nomeFile)+1 + strlen(ip)+1 + 48;
	char* get = (char*) malloc(lunghezza);
	printf("%i\n",strlen(nomeFile));		
	sprintf(get, "GET %s HTTP/1.1\r\nHost: %s\r\nUser-Agent: ClientHTTP/1.0\r\n\r\n", nomeFile, ip);
	printf("invio\n");	

	printf("%s", get);

	myself->invia("ciao");

	printf("invio riuscito, ricevo\n");
	ritorno = myself->ricevi();
	if(!strstr(ritorno, "200 OK")){
		printf("ERRORE RICEZIONE.\n");
		return -3;
	}
	printf("ricevuto\n");
	pagina = trovaPagina(ritorno);
	

	f = fopen(nomeFile,"w");
	fprintf(f, "%s\n",pagina);
	fclose(f);

	delete(myself);
	delete(server);
	free(nomeFile);
	free(pagina);
	free(ritorno);
	free(get);
	free(ip);
	return 0;

}

char* trovaPagina(char* s){
	char* ret;
	int len = strlen(s);
	int i=0;
	while((s[i]!='\n' || s[i+1]!='\n') && i<len-1){
		i++;
	}
	i+=2;

	for(int j=0;j<len-i;j++){
		ret[j] = s[i];
		i++;
	}
	return strdup(ret);
}
