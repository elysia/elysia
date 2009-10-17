
int grid_h = 100;
int grid_w = 100;
float soil_heat_exchange = 0.001;
float air_heat_exchange = 0.05;
float airground_heat_exchange = 0.002;
float air_ground_thermal_ratio = 0.1;
float sunlight_to_heat = 1;
float irradiance_to_space = 0.001;
float soil_waterflow_rate = 0.1;
float organic_wateflow_effect = 1;
float O2_diffusion = 0.001;
float CO2_diffusion = 0.001;
float H2O_diffusion = 0.001;
float evapoation_rate = 0.0001;
float unit_width = 100;

//SOIL
float z[grid_w][grid_h];
float organic[grid_w][grid_h];
float N[grid_w][grid_h];
float P[grid_w][grid_h];
float soil_H20[grid_w][grid_h];
float detritus[grid_w][grid_h];
float soil_temp[grid_w][grid_h];
float sunlight[grid_w][grid_h];

//AIR

float O2[grid_w][grid_h];
float CO2[grid_w][grid_h];
float N2[grid_w][grid_h];
float air_H20[grid_w][grid_h];
float air_temp[grid_w][grid_h];



for(i=0;i<grid_w;i++){
    for(j=0;j<grid_h;j++){
        //SOIL
        z[grid_w][grid_h] = 1;           //in meters
        organic[grid_w][grid_h] = 50;    //value
        N[grid_w][grid_h] = 0.5;         //From 0 to 1   (or value)
        P[grid_w][grid_h] = 0.5;         //From 0 to 1   (or value)
        soil_H20[grid_w][grid_h] = 0.5;  //From 0 to 1
        detritus[grid_w][grid_h] = 0;    //value
        soil_temp[grid_w][grid_h] = 25;  //in C
        sunlight[grid_w][grid_h] = 0.5;  //From 0 to 1
        O2[grid_w][grid_h] = 20;         //out of 100
        CO2[grid_w][grid_h] = 2;         //out of 100
        N2[grid_w][grid_h] = 78;         //out of 100
        air_H20[grid_w][grid_h] = 0.5;   //From 0 to 1, form clouds at 1
        air_temp[grid_w][grid_h] = 25;   //in C
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
