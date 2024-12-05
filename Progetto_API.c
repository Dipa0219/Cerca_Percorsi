#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int r=16384;
int numStation=0;
struct Nd{
    int autonomy;
    struct Nd * next;
};

typedef struct Nd* Ptr;


struct station {
    int km;
    struct Nd * car_list;
    int bestCar;
};

struct listStation {
    struct station * station;
    struct listStation * next;
};


int cmpfunc (const void * a, const void * b) {
    return ( *(int*)a - *(int*)b );
}
/**
 * Gives the position of a particular number in an array of int
 * @param track array of integer
 * @param km number to search
 * @return position of the number
 */
int searchPositionArray(const  int* track, int km){
    int i=0;
    while(*(track+i)!=-1){
        if(*(track+i) == km) return i;
        i++;
    }
    return -1;
}

/**
 * Hash function for the hash table
 * @param i
 * @param size
 * @return
 */
int hashFunction (int i){
    long double k;
    k=(long double )(sqrt(5)-1)/2;
    return floorl(r*((i*k)- floorl(i*k)));
}

/**
 * Print all the int number contained
 * @param journey array of int
 */
void printJourney(const int* journey){
    int i=0;
    if(*journey==-1){
        printf("nessun percorso\n");
        return;
    }
    while (*(journey+i+1)!=-1){
        printf("%d ", *(journey+i));
        i++;
    }
    printf("%d\n", *(journey+i));
}

/**
 * Add selected car to the corresponding station
 * @param station selected station
 * @param car selected car
 */
void addCar(struct station *station,int car){
    Ptr x=(Ptr) malloc (sizeof (struct Nd));
    x->autonomy=car;
    if (station->car_list==NULL){
        x->next=NULL;
        station->car_list=x;
        station->bestCar=car;
        return;
    }
    x->next=station->car_list;
    station->car_list=x;
    //printf("Best Car:%d, My car: %d\n", station->bestCar,car);
    if (station->bestCar<car) {
        //printf("Entrato\n");
        station->bestCar = car;
    }
}

int searchCar(struct Nd* carList){
    if(carList==NULL){
        return 0;
    }
    int bestCar=0;
    struct Nd* x= carList;
    while (x->next!=NULL){
        if(x->autonomy>bestCar){
            bestCar=x->autonomy;
        }
        x=x->next;
    }
    return bestCar;
}

/**
 * Remove the selected the station
 * @param station selected station
 * @param car car to add
 * @return 1 if the station has been removed, 0 if not
 */
int removeCar(struct station * station,int car){
    Ptr x= station->car_list;
    if (x==NULL) return 0;
    else if (x->autonomy== car ) {
        station->car_list=x->next;
        free(x);
        return 1;
    }
    else if(x->autonomy!= car && x->next==NULL){
        return 0;
    }
    else{
        while(x->next->next!=NULL){
            if(x->next->autonomy==car){
                Ptr temp=x->next;
                x->next=x->next->next;
                free(temp);
                return 1;
            }
            x=x->next;
        }
        if(x->next->autonomy==car){
            Ptr temp=x->next;
            x->next=NULL;
            free(temp);
            return 1;
        }
        return 0;
    }
}

/**
 * Prints all the cars in the list
 * @param carList list of cars
 */
void checkAllCar(struct Nd* carList){
    int i=1;
    struct Nd * temp=carList;
    if(carList==NULL) return;
    while (temp->next!=NULL) {
        printf("Auto %d: %d\n",i,temp->autonomy);
        i++;
        temp=temp->next;
    }
    printf("Auto %d: %d\n",i,temp->autonomy);
}


/**
 * Remove the selected the station and search the new best one
 * @param station hash list of station
 * @param car car to add
 */
void removeAndSearchCar(struct station * station,int car){
    Ptr x= station->car_list;
    if (x->autonomy == car ){
        station->bestCar=0;
        if(x->next==NULL) {
            free(x);
            station->car_list=NULL;
            return;
        }
        else{
            station->car_list=x->next;
            free(x);
            x=station->car_list;
            while(x->next!=NULL){
                if(x->autonomy > station->bestCar) station->bestCar= x->autonomy;
                x=x->next;
            }
            if(x->autonomy > station->bestCar) station->bestCar= x->autonomy;
            return;
        }
    }
    else{
        int founded=0;
        station->bestCar=x->autonomy;
        while(x->next->next!=NULL){
            if(x->next->autonomy == car && !founded){
                Ptr temp=x->next;
                x->next=x->next->next;
                free(temp);
                founded=1;
                if(x->next->next==NULL){
                    if(x->autonomy > station->bestCar) station->bestCar= x->autonomy;
                    break;
                }
            }
            if(x->autonomy > station->bestCar) station->bestCar= x->autonomy;
            x=x->next;
        }
        if(x->next->autonomy==car && !founded){
            Ptr temp=x->next;
            x->next=NULL;
            free(temp);
        }
        if(x->autonomy > station->bestCar) station->bestCar= x->autonomy;
        if(founded)
            if(x->next->autonomy > station->bestCar)
                station->bestCar= x->autonomy;
    }
}



/**
 * Finds the selected station in the hash list and returns its position
 * @param autostrada array which contains the hash table
 * @param station
 * @return position of the selected station
 */

int findStation (struct listStation* autostrada, int station){
    int temp = hashFunction(station);
    if((autostrada+temp)->station!=NULL){
        //printf("Entrato\n");
        if (station == (autostrada + temp)->station->km) { return temp; }
        struct listStation *prov = autostrada+temp;
        //printf("Arrivato %d\n",prov ->station->km);
        while (prov->next != NULL) {
            prov = prov->next;
            //printf("Perchè cazzo mi rompo? %d\n", prov ->station->km);
            if (station == prov ->station->km) { return temp; }
        }
    }
    return -1;
}

/**
 * Finds the selected station in the hash list and returns the selected station
 * @param autostrada array which contains the hash table
 * @param station
 * @return the selected station
 */

struct station* findCompleteStation(struct listStation* autostrada, int station){
    int temp = hashFunction(station);
    if((autostrada+temp)->station!=NULL){
        if (station == (autostrada + temp)->station->km) { return (autostrada + temp)->station; }
        struct listStation *prov = autostrada+temp;
        while (prov->next != NULL) {
            prov = prov->next;
            if (station == prov->station->km) { return prov->station; }
        }
    }
    return NULL;
}

/**
 * Adds the selected station to array with all the selected cars
 * @param autostrada array which contains the hash table
 * @param station km of the station
 * @return the array with all the modification
 */
void addStation(struct listStation * autostrada, int station){
    int temp = hashFunction(station);
    struct station * service= calloc(1,sizeof (struct station));
    service->km=station;
    int k;
    if(!scanf("%d", &k)) return;
    int j;
    for(int i=0;i<k;i++){
        if (!scanf("%d", &j)) return;
        addCar(service,j);
    }
    if ((autostrada+temp)->station==NULL) {
        (autostrada + temp)->station = service;
        (autostrada + temp)->next=NULL;
        return;
    }
    struct listStation* prov=autostrada+temp;
    struct listStation* listStation= malloc(sizeof(struct listStation));
    while(prov->next!=NULL) {
        prov = prov->next;
    }
    listStation->station=service;
    listStation->next=NULL;
    prov->next = listStation;
}

void freeAuto(struct Nd* carlist){
    while(carlist->next!=NULL){
        struct Nd* carToFree;
        carToFree=carlist;
        carlist=carlist->next;
        free(carToFree);
    }
    free(carlist);
}

/**
 * Removes the selected station from the hash table
 * @param autostrada array which contains the hash table
 * @param pos km of the station
 */

void removeStation (struct listStation * autostrada, int km){
    if ((autostrada)->station->km==km && (autostrada)->next==NULL){
        struct station *stationtoremove = autostrada->station;
        autostrada->station=NULL;
        freeAuto(stationtoremove->car_list);
        free(stationtoremove);
        return;
    }
    else if((autostrada)->station->km==km && (autostrada)->next!=NULL){
        struct listStation* temp= autostrada->next;
        struct station* station= autostrada->station;
        autostrada->station=autostrada->next->station;
        autostrada->next=autostrada->next->next;
        freeAuto(station->car_list);
        free(station);
        free(temp);
    }
    else{
        struct listStation* temp= autostrada;
        while(temp->next->next!=NULL){
            if(temp->next->station->km==km){
                struct listStation * prov =temp->next;
                temp->next=prov->next;
                freeAuto(prov->station->car_list);
                free(prov->station);
                free(prov);
                return;
            }
            temp=temp->next;
        }
        if(temp->next->station->km==km){
            struct listStation * prov =temp->next;
            temp->next=temp->next->next;
            freeAuto(prov->station->car_list);
            free(prov->station);
            free(prov);
        }
    }
}
/**
 * Returns an ordered array of station between two number
 * @param autostrada array which contains the hash table
 * @param min minimum station
 * @param max maximum station
 * @return ordered array
 */
int * stationToArray(struct listStation * autostrada , int min, int max){
    int * orderedStation;
    orderedStation = calloc(2*numStation+1, sizeof(int));
    //printf("NumStation: %d\n",numStation);
    int j=0;
    for(int i=0;i<r;i++){
        struct listStation* prov=(autostrada+i);
        if(prov->station!=NULL){
            while (prov->next != NULL) {
                if (prov->station->km >= min && prov->station->km <= max) {
                    *(orderedStation + j) = prov->station->km;
                    j++;
                }
                prov = prov->next;
            }
            if (prov->station->km >= min && prov->station->km <= max) {
                *(orderedStation + j) = prov->station->km;
                j++;
                //printf("j: %d, %d, %d\n",j, prov->station->km,prov->station->bestCar);
            }
        }
    }
    qsort(orderedStation,j,sizeof (int),cmpfunc);
    *(orderedStation+j)=-1;
    return orderedStation;
}

int searchMostDistantStation(const int* track, int bestCar){
    int i=1;
    while(*(track+i)!=-1){
        if(bestCar < *(track+i)- *track) return --i;
        i++;
    }
    return --i;
}

int searchMostDistantStationBack(const int* track,int pos, int bestCar){
    int i=pos-1;
    while(i>=0){
        if(bestCar < -(*(track+i)- *(track+pos))) return ++i;
        i--;
    }
    return ++i;
}

int * choseJourneyStraight(struct listStation * autostrada,const int* track, int start, int end){
    int* finalJourney;
    finalJourney= calloc(2*numStation+1,sizeof(int));
    int i=0;
    *(finalJourney+i)=start;
    i++;
    if(findCompleteStation(autostrada,start)->bestCar > (end - start)){
        *(finalJourney+i)=end;
        return finalJourney;
    }
    int posBase=0;
    int posBestStation= searchMostDistantStation(track,findCompleteStation(autostrada,start)->bestCar);
    //printf("Pos Base: %d, Pos Best Station: %d, Best Car: %d \n",*(track+posBase),*(track+posBestStation),
    //       findCompleteStation(autostrada,*(track+posBase))->bestCar);
    while(*(track+posBestStation) != end){
        int posToOvercome= posBestStation + searchMostDistantStation(track+posBestStation,findCompleteStation(autostrada,*(track+posBestStation))->bestCar);
        //printf("posToOvercome: %d\n",*(track+posToOvercome));
        for(int j=posBestStation-1;j>posBase;j--){
            if(findCompleteStation(autostrada,*(track+j))->bestCar >= *(track+posToOvercome) - *(track+j)) {
                //printf("Entrato per %d che ha macchina %d\n", *(track+j),findCompleteStation(autostrada,*(track+j))->bestCar);
                posBestStation = j;
                posToOvercome= posBestStation + searchMostDistantStation(track+posBestStation,findCompleteStation(autostrada,*(track+posBestStation))->bestCar);
            }
        }
        int t=i-1;
        int finalcheck=0;
        int distToCheck= *(track + posBestStation);
        while(t>=1 && !finalcheck){
            finalcheck=1;
            //printf("%d \n",t);
            int oldStation = searchPositionArray(track, *(finalJourney + t));
            int oldestStation = searchPositionArray(track, *(finalJourney + t-1));
            //printf("Old station: %d, Oldest station:%d\n",oldStation,oldestStation);
            for (int k = oldStation - 1; k > oldestStation; k--) {
                if (findCompleteStation(autostrada, *(track + k))->bestCar >= distToCheck - *(track + k)) {
                    //printf("%d is best than %d\n",*(track + k),*(finalJourney + t));
                    *(finalJourney + t) = *(track + k);
                    finalcheck=0;
                }
            }
            distToCheck= *(finalJourney + t);
            t=t-1;
        }
        *(finalJourney+i)=*(track+posBestStation);
        i++;
        if(posBestStation==posToOvercome){
            *finalJourney=-1;
            return finalJourney;
        }
        //printf("PrePosBase: %d\n",*(track+posBase));
        posBase= posBestStation;
        //printf("PosBase: %d\n",*(track+posBase));
        //printf("PrePosBestStation: %d\n",*(track+posBestStation));
        posBestStation= posToOvercome;
        //printf("PosBestStation: %d\n",*(track+posBestStation));
    }
    if((findCompleteStation(autostrada,*(finalJourney+i-1))->bestCar)>=(end-*(finalJourney+i-1))){
        *(finalJourney+i)=end;
        *(finalJourney+i+1)=-1;
    }
    return finalJourney;
}

int * choseJourneyBack(struct listStation * autostrada,int* track, int start, int end){
    int* finalJourney;
    finalJourney= calloc(2*numStation+1,sizeof(int));
    int i=0;
    *(finalJourney+i)=start;
    i++;
    if(start==end) return finalJourney;
    else if(findCompleteStation(autostrada,start)->bestCar > (start - end)){
        *(finalJourney+i)=end;
        return finalJourney;
    }
    int posBase = searchPositionArray(track,start);
    //printf("PosBase: %d\n",*(track+posBase));
    int posBestStation = searchMostDistantStationBack(track,posBase, findCompleteStation(autostrada,start)->bestCar);
    //printf("PosBestStation: %d\n",*(track+posBestStation));
    while(*(track+posBestStation) != end){
        int posToOvercome= searchMostDistantStationBack(track,posBestStation,findCompleteStation(autostrada,*(track+posBestStation))->bestCar);
        if(*(track+posToOvercome)==end) {
            *(finalJourney+i)=*(track+posBestStation);
            i++;
            break;
        }
        //printf("posToOvercome: %d\n",*(track+posToOvercome));
        for (int j = posBestStation+1;j < posBase;j++){
            if(findCompleteStation(autostrada,*(track+j))->bestCar >= -(*(track+posToOvercome-1) - *(track+j))){
                posBestStation = j;
                posToOvercome= searchMostDistantStationBack(track,posBestStation,findCompleteStation(autostrada,*(track+posBestStation))->bestCar);
                //printf("newPosToOvercome: %d,PosBestStation: %d\n",*(track+posToOvercome),*(track+posBestStation));
            }
            if(*(track+posToOvercome)==end) break;
        }
        *(finalJourney+i)=*(track+posBestStation);
        i++;
        if(posBestStation==posToOvercome){
            *finalJourney=-1;
            return finalJourney;
        }
        posBase= posBestStation;
        //printf("PosBase: %d\n",*(track+posBase));
        posBestStation= posToOvercome;
        //printf("PosBestStation: %d\n",*(track+posBestStation));
    }
    if((findCompleteStation(autostrada,*(finalJourney+i-1))->bestCar)>=-(end-*(finalJourney+i-1))){
        *(finalJourney+i)=end;
        *(finalJourney+i+1)=-1;
    }

    while(i> 1){
        posBestStation= searchPositionArray(track,*(finalJourney+i));
        int posLastFoundStation = searchPositionArray(track, *(finalJourney + i - 1));
        int posBeforeStation= searchPositionArray(track,*(finalJourney+i-2));
        int limitPos= searchMostDistantStationBack(track,posBeforeStation, findCompleteStation(autostrada,*(finalJourney+i-2))->bestCar);
        for (int j = posLastFoundStation; j >= limitPos; j--) {
            //printf("Best car %d, Station to check: %d, Best station: %d\n",findCompleteStation(autostrada, *(track + j))->bestCar, *(track + j), *(track + posBestStation));
            if (findCompleteStation(autostrada, *(track + j))->bestCar >=*(track + j) - *(track + posBestStation)) {
                //printf("Entrato\n");
                *(finalJourney + i - 1) = *(track + j);
            }
        }
        i--;
    }
    return finalJourney;
}

struct listStation * autostrada;
char * input;

int main() {
    int pos;
    input= calloc(20,sizeof (char));
    autostrada = calloc(r,sizeof (struct listStation));
    while(scanf("%s", input)!=EOF){
        if (!scanf("%d", &pos)) { break; }
        int i= findStation(autostrada,pos);
        if(!strcmp(input,"aggiungi-stazione")) {
            if (i!=-1) {
                printf("non aggiunta\n");
                int k;
                if (!scanf("%d", &k)) break;
                int j;
                for (int m = 0; m < k; m++) {
                    if (!scanf("%d", &j)) break;
                }
            }
            else{
                addStation(autostrada, pos);
                printf("aggiunta\n");
                numStation++;
                //checkAllCar((findCompleteStation(autostrada,pos)->car_list));
                //printf("%d, %d, %d \n", (findCompleteStation(autostrada,pos))->km, hashFunction(pos),r);
                //if ((findCompleteStation(autostrada,pos)->car_list!=NULL))
                //    printf("Last car :%d, Best car: %d\n", (findCompleteStation(autostrada,pos))->car_list->autonomy,(findCompleteStation(autostrada,pos))->bestCar);
            }
        }
        else if (!strcmp(input,"demolisci-stazione")){
            if (i==-1)
                printf("non demolita\n");
            else{
                removeStation(autostrada+i,pos);
                printf("demolita\n");
                // printf("%d, %d, %d \n", (autostrada + hashFunction(pos))->km, hashFunction(pos),r);
            }
        }
        else if(!strcmp(input,"aggiungi-auto")){
            int j;
            if(!scanf("%d",&j)) break;
            if (i==-1)
                printf("non aggiunta\n");
            else {
                addCar(findCompleteStation(autostrada,pos), j);
                //checkAllCar(findCompleteStation(autostrada,pos)->car_list);
                //printf("Best auto: %d\n",findCompleteStation(autostrada,pos)->bestCar);
                printf("aggiunta\n");
            }
        }
        else if(!strcmp(input,"rottama-auto")){
            int j;
            if(!scanf("%d",&j)) break;
            struct station * station=findCompleteStation(autostrada,pos);
            if (station==NULL){
                printf("non rottamata\n");
            }
            else if(station->bestCar!=j){
                if(removeCar(station, j)){
                    printf("rottamata\n");
                    //checkAllCar(findCompleteStation(autostrada,pos)->car_list);
                    //printf("Best auto: %d\n",findCompleteStation(autostrada,pos)->bestCar);
                }
                else
                    printf("non rottamata\n");
            }
            else{
                //printf("Best car is %d and we remove %d, quindi entro 2\n",station->bestCar,j);
//                removeAndSearchCar(station,j);
                removeCar(station, j);
                station->bestCar= searchCar(station->car_list);
                //checkAllCar(findCompleteStation(autostrada,pos)->car_list);
                //printf("Best auto: %d\n",findCompleteStation(autostrada,pos)->bestCar);
                printf("rottamata\n");
            }
        }
        else if(!strcmp(input,"pianifica-percorso")){
            int* orderedList=NULL, *result=NULL;
            int end;
            if(!scanf("%d", &end)) break;
            if(end>pos) {
                orderedList = stationToArray(autostrada,pos, end);
                result = choseJourneyStraight(autostrada, orderedList, pos, end);
            }
            else {
                orderedList = stationToArray(autostrada,end, pos);
                result = choseJourneyBack(autostrada, orderedList, pos, end);
            }
            printJourney(result);
            free(orderedList);
            free(result);
        }
        else{
            continue;
        }
    }
    return 0;
}