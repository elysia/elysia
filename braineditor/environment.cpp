

soil_heat_exchange = 0.001;
air_heat_exchange = 0.05;
airground_heat_exchange = 0.002;
air_ground_thermal_ratio = 0.1;
sunlight_to_heat = 1;
irradiance_to_space = 0.001;
soil_waterflow_rate = 0.1;
organic_wateflow_effect = 1;
O2_diffusion = 0.001;
CO2_diffusion = 0.001;
H2O_diffusion = 0.001;
evapoation_rate = 0.0001;
width = 100;

//SOIL
float z[100][100];
float organic[100][100];
float N[100][100];
float P[100][100];
float soil_H20[100][100];
float detritus[100][100];
float soil_temp[100][100];
float sunlight[100][100];

//AIR

float O2[100][100];
float CO2[100][100];
float N2[100][100];
float air_H20[100][100];
float air_temp[100][100];


for(i=0;i<100;i++){
    for(j=0;j<100;j++){
        //SOIL
        z[100][100] = 1;           //in meters
        organic[100][100] = 50;    //value
        N[100][100] = 0.5;         //From 0 to 1   (or value)
        P[100][100] = 0.5;         //From 0 to 1   (or value)
        soil_H20[100][100] = 0.5;  //From 0 to 1
        detritus[100][100] = 0;    //value
        soil_temp[100][100] = 25;  //in C
        sunlight[100][100] = 0.5;  //From 0 to 1
        O2[100][100] = 20;         //out of 100
        CO2[100][100] = 2;         //out of 100
        N2[100][100] = 78;         //out of 100
        air_H20[100][100] = 0.5;   //From 0 to 1, form clouds at 1
        air_temp[100][100] = 25;   //in C
    }
}


for(i=0;i<100;i++){
    for(j=0;j<100;j++){
        //soil temperature exchange
        soil_heat_difference[i][j] -= soil_heat_exchange*(soil_temp[i][j] - soil_temp[i+1][j]);
        soil_heat_difference[i][j] -= soil_heat_exchange*(soil_temp[i][j] - soil_temp[i][j+1]);
        soil_heat_difference[i+1][j] -= soil_heat_exchange*(soil_temp[i+1][j] - soil_temp[i][j]);
        soil_heat_difference[i][j+1] -= soil_heat_exchange*(soil_temp[i][j+1] - soil_temp[i][j]);
        //air temperature exchange
        air_heat_difference[i][j] -= air_heat_exchange*(air_temp[i][j] - air_temp[i+1][j]);
        air_heat_difference[i][j] -= air_heat_exchange*(air_temp[i][j] - air_temp[i][j+1]);
        air_heat_difference[i+1][j] -= air_heat_exchange*(air_temp[i+1][j] - air_temp[i][j]);
        air_heat_difference[i][j+1] -= air_heat_exchange*(air_temp[i][j+1] - air_temp[i][j]);
        //air-soil heating
        soil_heat_difference[i][j] -= air_ground_thermal_ratio*airground_heat_exhcange*(soil_temp[i][j] - air_temp[i][j]);
        air_heat_difference[i][j] -= airground_heat_exhcange*(air_temp[i][j] - soil_temp[i][j])/air_ground_thermal_ratio;
        //solar heating
        soil_heat_difference[i][j] += sunlight_to_heat*air_ground_thermal_ratio*soil_temp[i][j];
        air_heat_difference[i][j] += sunlight_to_heat*air_temp[i][j];
        //irradiance to space
        air_heat_difference[i][j] -= irradiance_to_space*air_temp[i][j];
        
        //water flow soil
        soil_H20_difference[i][j] -= soil_waterflow_rate*log10(organic[$i][$j])*(1-(soil_H20[i][j] - soil_H20[i+1][j]));
        air_H20_difference +=  
        //evaporation
        //faster at higher ground T, air_T, lower humidity
        soil_H20_difference[i][j] -= soil_H20[i][j]*evaporation_rate*soil_temp[i][j]*(1-air_H2O[i][j]);
        air_H20_difference[i][j] += soil_H20[i][j]*evaporation_rate*soil_temp[i][j]*(1-air_H2O[i][j]);
        
        //water_flow_air
        air_H2O_difference[i][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i+1][j]);
        air_H2O_difference[i][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i][j+1]);        
        air_H2O_difference[i+1][j] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i][j+1]);
        air_H2O_difference[i][j+1] -= H2O_diffusion*(air_H2O[i][j] - air_H2O[i+1][j]);
        
        //air flow
        O2_difference[i][j] -= O2_diffusion*(O2[i][j] - O2[i+1][j]);
        O2_difference[i][j] -= O2_diffusion*(O2[i][j] - O2[i][j+1]);
        O2_difference[i+1][j] -= O2_diffusion*(O2[i+1][j] - O2[i][j]);
        O2_difference[i][j+1] -= O2_diffusion*(O2[i][j+1] - O2[i][j]);
        CO2_difference[i][j] -= CO2_diffusion*(CO2[i][j] - CO2[i+1][j]);
        CO2_difference[i][j] -= CO2_diffusion*(CO2[i][j] - CO2[i][j+1]);
        CO2_difference[i+1][j] -= CO2_diffusion*(CO2[i+1][j] - CO2[i][j]);
        CO2_difference[i][j+1] -= CO2_diffusion*(CO2[i][j+1] - CO2[i][j]);
        
        //nutrient_flow
        



    }
}


/* other functions

erosion
