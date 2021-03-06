#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>

#include "agent.h"
#include "Obstacle.h"


#include "controller.h"
//#include "controllerNSB.h"
#include "controllerCoop.h"
//#include "controllerComp.h"
//#include "controllerStationary.h"

#include "behaviourFollowHeading.h"
#include "behaviourRandom.h"
#include "behaviourTangential.h"
#include "behaviourAvoid.h"
//#include "behaviourSmartAvoid.h"
#include "behaviourDynamicFC.h"

#include "Vector2d.h"

#include "sensorContact.h"

using namespace std;

// Global constants
const int nAgents = 20;
const int nTeams = 4;
int agentsPerTeam = nAgents/nTeams;
int nObstacles = 0;
int maxTimeSteps = 5000;
float dt = 0.05;
//float dt = 0.01;
int outputFreq = 10;
float errorBound = 0.1;
float deploymentRadius = 10.;

///////////////////////////////////////////////////////////////////////////////
// Static pattern formation information
///////////////////////////////////////////////////////////////////////////////


// Booleans used to determine when formations achieved
bool formationAchieved[nTeams];
float distanceTravelled[nAgents];

//Function prototypes;

float Random(float a = 1.0);

void UpdateAgentSensors(Agent** agentArray, vector<Obstacle*> obstacleList);
void UpdateAgentVelocityCommands(Agent** agentArray);
void UpdateAgentPositions(Agent** agentArray);

void SaveAgentPositions(Agent** agentArray, int timeStep);
void SaveObstaclePositions(vector<Obstacle*> obstacleList);
bool RecordData(Agent** agentArray, int timeStep);

void RecordDistances(Agent** agentArray);


//*******************************************************************//
int main(){
  
  // Time step used for simulation 0.2 seconds.
  int currentTimeStep = 0;

  // initialize random seed: 
  srand (time(NULL));

  for(int i = 0; i < nTeams; i++){
    formationAchieved[i] = false;
  }
  for(int i = 0; i < nAgents; i++){
	distanceTravelled[i] = 0.;
   }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CREATE OBSTACLES
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  vector<Obstacle*> obstacleList;


  for(int i = 0; i < nObstacles; i++){
    Vector2d* pos =new Vector2d( Random(20.) + 60., Random(40.) );
    //Vector2d* pos =new Vector2d( 0. , 0. );
    Obstacle * f =  new Obstacle( pos, 1.);
    obstacleList.push_back(f);
  }

  SaveObstaclePositions(obstacleList);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // CREATE THE AGENTS, THEIR CONTROLLERS AND BEHAVIOURS
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  Agent** agentArray;
  agentArray = new Agent*[nAgents];
  int teamID = -1;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Static Neighbour selection
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /*

   for(int i = 0; i < nAgents; i++){
   
    if( i % (agentsPerTeam) == 0 ){
      teamID++;
    }

    ControllerCoop* aController = new ControllerCoop();

    //    aController->AddBehaviour( new BehaviourRandom(0.1) );  
    aController->AddBehaviour( new BehaviourAvoid(2.0) );
    
    // Linear Formation
    if(i == 0) 
      aController->AddBehaviour( new BehaviourTangential(M_PI/2., 1) );
    else if (i == nAgents-1){
      aController->AddBehaviour( new BehaviourTangential(-M_PI/2., nAgents-2) );
    }
    else{
      aController->AddBehaviour( new BehaviourTangential(M_PI/2., i+1) );
      aController->AddBehaviour( new BehaviourTangential(-1.*M_PI/2., i-1) );
     }

    // Other formation 
    //aController->AddBehaviour( new BehaviourTangential(bearingConstArray[2*i]*M_PI, targetIDArray[2*i]) );
    //aController->AddBehaviour( new BehaviourTangential(bearingConstArray[2*i+1]*M_PI, targetIDArray[2*i+1]) );
    
    // Create Agent
    agentArray[i] = 
      new Agent( new Vector2d( Random(deploymentRadius), Random(deploymentRadius) ), 0., teamID, aController ); 

  }/**/

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Dynamic Neighbour selection
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  /**/
  for(int i = 0; i < nAgents; i++){
    teamID = 0;
    ControllerCoop* aController = new ControllerCoop();

    //aController->AddBehaviour( new BehaviourRandom(0.1) );  
    //aController->AddBehaviour( new BehaviourAvoid(2.0) );
    //aController->AddBehaviour( new BehaviourAvoid(-3.0) ); 
    //aController->AddBehaviour( new BehaviourFollowHeading(0., 0.5) );
  
    float DA_Zone = 1.9;

    // Line formation
    //aController->AddBehaviour( new BehaviourDynamicFC( 0.*M_PI,  DA_Zone) );

    // Wedge Formations.
    //if( i < nAgents/2){
    //  aController->AddBehaviour( new BehaviourDynamicFC( 0.25*M_PI,  DA_Zone) );
    //  teamID = 0;
    // }
    // else{
    //  aController->AddBehaviour( new BehaviourDynamicFC( -0.25*M_PI,  DA_Zone) );
    // teamID = 1;
    //}

    // Square formations    
    if(i < 5){
      aController->AddBehaviour( new BehaviourDynamicFC( 0.*M_PI,  DA_Zone) );
      teamID = 0;
    }
    else if ( i < 10){
      aController->AddBehaviour( new BehaviourDynamicFC( 0.5*M_PI,  DA_Zone) );
      teamID = 1;
    }
    else if ( i < 15){
      aController->AddBehaviour( new BehaviourDynamicFC( M_PI,  DA_Zone) );
      teamID = 2;
    }
    else{
      aController->AddBehaviour( new BehaviourDynamicFC( -0.5*M_PI, DA_Zone ) );
      teamID = 3;
    }
       
    agentArray[i] = 
      new Agent( new Vector2d( Random(deploymentRadius) , Random(deploymentRadius) ), 0.,  teamID, aController ); 
  }/**/
    
  // SAVE INITIAL POSITIONS
  SaveAgentPositions(agentArray, 0);
  RecordData(agentArray, 0);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // START OF THE MAIN SIMULATION LOOP
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool first = true;
  while(currentTimeStep < maxTimeSteps){

    // printf("Time Step: %d\n", currentTimeStep);
    
    //printf("Update Sensors.\n");
    UpdateAgentSensors(agentArray, obstacleList);

    //printf("Update Veclocities.\n");    
    UpdateAgentVelocityCommands(agentArray);  
    
    //printf("Update Positions.\n");
    UpdateAgentPositions(agentArray);

    //printf("Simulation cycle complete!\n\n");
  
    if(currentTimeStep % outputFreq == 0) {
      SaveAgentPositions(agentArray, currentTimeStep);
      RecordDistances(agentArray);
  
      bool result = RecordData(agentArray, currentTimeStep);
      if(result and first){
	printf("Time = %d\n",currentTimeStep);
	first = false;
      }

    }

    currentTimeStep++;
    //getchar();
  }
 
  return 0;
 
};

/*******************************************************************************/

float Random(float a){
  return a*rand()/(float)(RAND_MAX)-a/2.;
}

void UpdateAgentSensors(Agent** agentArray, vector<Obstacle*> obstacleList){


  // Loop over all agents and calculate the distance and bearing
  // to each contact
  for(int i = 0; i < nAgents; i++){
    
    std::vector<SensorContact*> contacts;
   
    // First loop over all agents.
    for(int d = 0; d < nAgents; d++){
      if(i != d){
	Vector2d * aVector = SubtractVectors(agentArray[d]->GetPosition(), 
					     agentArray[i]->GetPosition());	
	char teamID = '0' + agentArray[d]->GetTeamID();
	contacts.push_back( new SensorContact(teamID, aVector) );
	//contacts.push_back( new SensorContact('f', aVector) );
      }
      else{
	Vector2d * aVector = new Vector2d(0.,0.);
	char teamID = '0' + agentArray[d]->GetTeamID();

	contacts.push_back( new SensorContact(teamID, aVector) );
	//contacts.push_back( new SensorContact('f', aVector) );
      }
    }
    
    // Now loop over all obstacles
    for(int d = 0; d < obstacleList.size(); d++){
      Vector2d * aVector = SubtractVectors(obstacleList[d]->GetPosition(), 
					     agentArray[i]->GetPosition());
      
      // Subtract the radius of the obstacle
      float dist = aVector->GetNorm();
      aVector->Normalize();

      aVector = ScalarMultipleVector(dist - obstacleList[d]->GetRadius(), aVector);

      //printf("dist = %f\n", dist- obstacleList[d]->GetRadius());

      contacts.push_back( new SensorContact('o', aVector) );
    }

    
    // Now that all the agents and obstacles have been detected send
    // contact list to the i^th agents
    agentArray[i]->UpdateSensors(contacts);
  }

}

void UpdateAgentVelocityCommands(Agent** agentArray){
   for(int i = 0; i < nAgents; i++)
     agentArray[i]->UpdateVelocity();
}

void UpdateAgentPositions(Agent** agentArray){
   for(int i = 0; i < nAgents; i++)
     agentArray[i]->UpdatePosition(dt);
}

void SaveAgentPositions(Agent** agentArray, int timeStep){
  std::fstream fs;
  std::ostringstream filename;
  
  if(timeStep < 10)
    filename <<"../output/rawData/time000" <<timeStep;
 else if(timeStep < 100)
    filename <<"../output/rawData/time00" <<timeStep;
 else if(timeStep < 1000) 
   filename <<"../output/rawData/time0" <<timeStep;
 else
   filename <<"../output/rawData/time" <<timeStep;
  
  fs.open (filename.str().c_str(), std::fstream::out);

  for(int i = 0; i < nAgents; i++){
      
    Vector2d * pos = agentArray[i]->GetPosition();
    int colour = agentArray[i]->GetTeamID();
    fs << std::setprecision(3) << i<<"\t"<<pos->x <<"\t" << pos->y<< "\t"<<colour+1 <<"\t";
    
    std::vector<Vector2d*> responseVectors = agentArray[i]->GetLastBehaviourResponses();
    if(timeStep == 0){
      fs <<"\n";     
      continue;
    }
    for(int i = 0; i < responseVectors.size(); i++){
      float norm = responseVectors[i]->GetNorm();
      float ang = responseVectors[i]->GetAngle();
      fs <<std::setprecision(3)<<norm <<"\t"<<ang/M_PI <<"\t";
      }
    fs <<"\n";
  }

  fs.close();
}

void SaveObstaclePositions(vector<Obstacle*> obstacleList){
  std::fstream fs;
  std::ostringstream filename;
  filename <<"../output/rawData/obstacles";

  fs.open (filename.str().c_str(), std::fstream::out);

  for(int i = 0; i < obstacleList.size(); i++){
     Vector2d * pos = obstacleList[i]->GetPosition();
     float r = obstacleList[i]->GetRadius(); 

     fs << pos->x <<"\t" << pos->y << "\t" << r << "\n";
  }

}

bool RecordData(Agent** agentArray, int timeStep){
  std::fstream fs;
  std::ostringstream filename;
  
  // Calculate a seperate statistic for each team
  float *averageX = new float[nTeams];
  float *averageY = new float[nTeams];
  float *stdX = new float[nTeams];
  float *stdY = new float[nTeams];

  // Zero the arrays
  for(int i = 0; i < nTeams; i++){
    averageX[i] = 0.;
    averageY[i] = 0.;
    stdX[i] = 0.;
    stdY[i] = 0.;
  }

  // Open data file
  filename <<"../output/rawData/Data_vs_Time.dat";
  fs.open (filename.str().c_str(), std::fstream::in | std::fstream::out | std::fstream::app);

  // Sum x and y coords of each team
  for(int i = 0; i < nAgents; i++){
    Vector2d * pos = agentArray[i]->GetPosition();
    int teamID = agentArray[i]->GetTeamID();
    
    averageX[teamID] += pos->x;
    averageY[teamID] += pos->y;
  }

  
  for(int i = 0; i < nTeams; i++){
    averageX[i] /= agentsPerTeam;
    averageY[i] /= agentsPerTeam;
  }

  for(int i = 0; i < nAgents; i++){
    Vector2d * pos = agentArray[i]->GetPosition();
    int teamID = agentArray[i]->GetTeamID();
    
    stdX[teamID] += pow(pos->x - averageX[teamID],2);
    stdY[teamID] += pow(pos->y - averageY[teamID],2);
  }
  
  for(int i = 0; i < nTeams; i++){
    stdX[i] = sqrt(stdX[i]);
    stdY[i] = sqrt(stdY[i]);
  }
  
  for(int i = 0; i < nTeams; i++){
    
    // Good test for line formation along axis
    if(i %2 == 0){
      if(stdX[i] < errorBound)
	formationAchieved[i] = true;
    }
    else{
      if(stdX[i] < errorBound)
	formationAchieved[i] = true;
    }
    fs << i <<"\t"<< timeStep <<"\t"<<averageX[i] << "\t" << averageY[i] <<"\t"<< stdX[i] <<"\t"<< stdY[i] <<"\n";

  }

  
  bool returnVal = true;
  
  for(int i = 0; i < nTeams; i++){
    returnVal *= formationAchieved[i];
  }

  fs.close();

  /*if(timeStep == maxTimeSteps-outputFreq){
    printf("TeamID\tstdX\tstdY\n");
    for(int i = 0; i < nTeams; i++){
      printf("%i\t%f\t%f\n",i, stdX[i], stdY[i],);
    }
    }*/
 
 return returnVal;
}
  
void RecordDistances(Agent** agentArray){

  std::fstream fs;
  std::ostringstream filename;
  
  filename <<"../output/rawData/distanceTravelled.dat";
  fs.open (filename.str().c_str(), std::fstream::out);
  
  float averageDistanceTravelled = 0.;
  float totalDistanceTravelled = 0.;

  // Average distance calculation;
   for(int i = 0; i < nAgents; i++){
     float distanceTravelled = agentArray[i]->GetDistanceTravelled();
     
     //printf("%.2f\t",distanceTravelled);

     averageDistanceTravelled += fabs(distanceTravelled);
     
     totalDistanceTravelled += fabs(distanceTravelled);
     
     // fs << i << "\t" <<distanceTravelled<<"\n";
     fs<<distanceTravelled<<"\n";
   }
  

   averageDistanceTravelled /= nAgents;
   // fs <<"Avg. Dist =\t" << averageDistanceTravelled <<"\n";
   fs << averageDistanceTravelled <<"\n";
   fs << totalDistanceTravelled <<"\n";
   fs.close(); 

   printf("%.2f\t%.2f\t",totalDistanceTravelled, averageDistanceTravelled);
   printf("\n");
}
 
