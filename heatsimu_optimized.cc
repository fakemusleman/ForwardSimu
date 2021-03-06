/*
  ************************************************************************************
  *****Heat Transfer Simulation for Real Time Grain Growth Control of Copper Film*****
  ************************************************************************************

  This work was supported by NSF DMREF program with reference codes: 024E, 036E, 8022, AMPP

  Project Investigators:
  Robert Hull hullr2@rpi.edu (Principal Investigator)
  John Wen wenj@rpi.edu (Co-Principal Investigator)
  Antoinette Maniatty maniaa@rpi.edu (Co-Principal Investigator)
  Daniel Lewis lewisd2@rpi.edu (Co-Principal Investigator)

  Code developer: Yixuan Tan tany3@rpi.edu

  Copyright (C) 2014 Rensselaer Polytechnic Institute.
  This work is distributed under the terms and conditions of the Rensselaer Maniatty Public License Version 1.0.
*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <fstream>

class Constants{
public:
  static double kLengthOfSquareDomain_;
  static int kNumOfHeaters_;
  static double kStefanBoltzmann_;
  static int kNumOfNodesInElement_;
  static int kNumOfDofsPerNode_;
  static int kNumOfMaterials_;
  static double kNormTolerance_;
  static double kYFunctionTolerance_;
  static int kMaxNewtonIteration_;
  static int kMeshSeedsAlongSiliconThickness_;
  static int kMeshSeedsAlongSTitaniumThickness_;
  static double kMinYCoordinate_;
};
double Constants::kLengthOfSquareDomain_ = 1.0;
int Constants::kNumOfHeaters_ = 10;
double Constants::kStefanBoltzmann_=5.6703e-11; // units mW/(mm^2 * K^4)   5.6703e-8 W*m^-2*K^-4
int Constants::kNumOfNodesInElement_=4;
int Constants::kNumOfDofsPerNode_=1;
int Constants::kNumOfMaterials_=4;
double Constants::kNormTolerance_=1.0e-5;
double Constants::kYFunctionTolerance_=1.0e-5;
int Constants::kMaxNewtonIteration_=10;
int Constants::kMeshSeedsAlongSiliconThickness_=5;
int Constants::kMeshSeedsAlongSTitaniumThickness_=1;
double Constants::kMinYCoordinate_=0.0;


class ModelGeometry{
public:
  void set_length_of_model() 
    {length_of_model_=Constants::kLengthOfSquareDomain_+2*width_of_end_;}
  void set_thickness_of_csilicon(const double thickness_of_csilicon) 
    {thickness_of_csilicon_=thickness_of_csilicon;}
  void set_thickness_of_titanium(const double thickness_of_titanium) 
    {thickness_of_titanium_=thickness_of_titanium;}
  void set_thickness_of_isolater(const double thickness_of_isolater) 
    {thickness_of_isolater_=thickness_of_isolater;}
  void set_thickness_of_silicondioxide(const double thickness_of_silicondioxide) 
    {thickness_of_silicondioxide_=thickness_of_silicondioxide;}
  void set_thickness_of_copper(const double thickness_of_copper) 
    {thickness_of_copper_=thickness_of_copper;}
  void set_width_of_end(const double width_of_end) 
    {width_of_end_=width_of_end;}
  void set_new_width_of_end(const double width_of_end) 
    {width_of_end_=width_of_end;}
  void set_width_of_heater(const double width_of_heater) 
    {width_of_heater_=width_of_heater;}
  void set_width_of_gap()
    {width_of_gap_= Constants::kLengthOfSquareDomain_/Constants::kNumOfHeaters_ - width_of_heater_;}
  double get_length_of_model() const 
    {return length_of_model_;}
  double get_thickness_of_csilicon() const
    {return thickness_of_csilicon_;}
  double get_thickness_of_titanium() const 
    {return thickness_of_titanium_;}
  double get_thickness_of_isolater() const 
    {return thickness_of_isolater_;}
  double get_thickness_of_silicondioxide() const 
    {return thickness_of_silicondioxide_;}
  double get_thickness_of_copper() const 
    {return thickness_of_copper_;}
  double get_width_of_end() const 
    {return width_of_end_;}
  double get_width_of_heater() const 
    {return width_of_heater_;}
  double get_width_of_gap() const 
    {return width_of_gap_;}

private:
  double length_of_model_;
  double thickness_of_csilicon_;
  double thickness_of_titanium_;
  double thickness_of_isolater_;
  double thickness_of_silicondioxide_;
  double thickness_of_copper_;
  double width_of_end_;
  double width_of_heater_;
  double width_of_gap_;
};


class AnalysisConstants{
public:
  void set_ambient_temperature(const double ambient_temperature)
    {ambient_temperature_=ambient_temperature; }
  void set_boundary_condition_temperature(const double boundary_condition_temperature)      
    {boundary_condition_temperature_=boundary_condition_temperature; }
  void set_output_time_step_interval(const int output_time_step_interval)
    {output_time_step_interval_=output_time_step_interval;}
  void set_sample_initial_temperature(const double sample_initial_temperature)
    {sample_initial_temperature_=sample_initial_temperature;}
  void set_maximum_time_steps(const int maximum_time_steps)
    {maximum_time_steps_=maximum_time_steps;}
  void set_total_simulation_time(const double total_simulation_time)
    {total_simulation_time_=total_simulation_time;}
  void set_initial_time_increment(const double initial_time_increment)
    {initial_time_increment_=initial_time_increment;}
  void set_minimum_time_increment(const double minimum_time_increment)
    {minimum_time_increment_=minimum_time_increment;}
  void set_time_to_turn_off_heaters(const double time_to_turn_off_heaters)
    {time_to_turn_off_heaters_=time_to_turn_off_heaters;}
  void set_maximum_temperature_change_per_time_increment(const double maximum_temperature_change_per_time_increment)
    {maximum_temperature_change_per_time_increment_=maximum_temperature_change_per_time_increment;}
  double get_boundary_condition_temperature() const       
    {return boundary_condition_temperature_; }
  double get_ambient_temperature() const 
    {return ambient_temperature_; }
  double get_time_to_turn_off_heaters() const 
    {return time_to_turn_off_heaters_;}
  double get_output_time_step_interval() const 
    {return output_time_step_interval_;}
  double get_sample_initial_temperature() const 
    {return sample_initial_temperature_;}
  int get_maximum_time_steps() const 
    {return maximum_time_steps_;}
  double get_total_simulation_time() const 
    {return total_simulation_time_;}
  double get_initial_time_increment() const 
    {return initial_time_increment_;}
  double get_minimum_time_increment() const 
    {return minimum_time_increment_;}
  double get_maximum_temperature_change_per_time_increment() const 
    {return maximum_temperature_change_per_time_increment_;}

private:
  double ambient_temperature_;
  double boundary_condition_temperature_;
  double time_to_turn_off_heaters_;
  int output_time_step_interval_;
  double sample_initial_temperature_;
  int maximum_time_steps_; 
  double total_simulation_time_;
  double initial_time_increment_;
  double minimum_time_increment_;
  double maximum_temperature_change_per_time_increment_;
};


class MeshParameters{
public:
  void set_mesh_seeds_on_end(const int mesh_seeds_on_end)
    {mesh_seeds_on_end_=mesh_seeds_on_end;}
  void set_mesh_seeds_on_gap(const int mesh_seeds_on_gap)
    {mesh_seeds_on_gap_=mesh_seeds_on_gap;}
  void set_mesh_seeds_on_heater(const int mesh_seeds_on_heater)
    {mesh_seeds_on_heater_=mesh_seeds_on_heater;}
  void set_mesh_seeds_along_isolater_thickness(const int mesh_seeds_along_isolater_thickness)
    {mesh_seeds_along_isolater_thickness_=mesh_seeds_along_isolater_thickness;}
  void set_mesh_seeds_along_silicondioxide_thickness(const int mesh_seeds_along_silicondioxide_thickness)
    {mesh_seeds_along_silicondioxide_thickness_=mesh_seeds_along_silicondioxide_thickness;}
  void set_mesh_seeds_along_copper_thickness(const int mesh_seeds_along_copper_thickness)
    {mesh_seeds_along_copper_thickness_=mesh_seeds_along_copper_thickness;}
  int get_mesh_seeds_on_end() const 
    {return mesh_seeds_on_end_;}
  int get_mesh_seeds_on_gap() const 
    {return mesh_seeds_on_gap_;}
  int get_mesh_seeds_on_heater() const 
    {return mesh_seeds_on_heater_;}
  int get_mesh_seeds_along_isolater_thickness() const 
    {return mesh_seeds_along_isolater_thickness_;}
  int get_mesh_seeds_along_silicondioxide_thickness() const 
    {return mesh_seeds_along_silicondioxide_thickness_;}
  int get_mesh_seeds_along_copper_thickness() const 
    {return mesh_seeds_along_copper_thickness_;}
  void set_dimensions_of_x()
    {dimensions_of_x_=Constants::kNumOfHeaters_*(mesh_seeds_on_heater_+mesh_seeds_on_gap_)+2*mesh_seeds_on_end_+1;}
  int get_dimensions_of_x() const 
    {return dimensions_of_x_;}
  void set_dimensions_of_y(){
    dimensions_of_y_ = Constants::kMeshSeedsAlongSiliconThickness_+Constants::kMeshSeedsAlongSTitaniumThickness_+
                       mesh_seeds_along_silicondioxide_thickness_+mesh_seeds_along_isolater_thickness_+mesh_seeds_along_copper_thickness_+1;
  }
  int get_dimensions_of_y() const 
    {return dimensions_of_y_;}
  int set_num_of_nodes()
    {num_of_nodes_=dimensions_of_x_*dimensions_of_y_;}
  int get_num_of_nodes() const 
    {return num_of_nodes_;}
  int set_num_of_elements()
    {num_of_elements_=(dimensions_of_x_-1)*(dimensions_of_y_-1);}
  int get_num_of_elements() const 
    {return num_of_elements_;}
 
private:
  int mesh_seeds_on_end_;
  int mesh_seeds_on_gap_;
  int mesh_seeds_on_heater_;  
  int mesh_seeds_along_isolater_thickness_;
  int mesh_seeds_along_silicondioxide_thickness_;
  int mesh_seeds_along_copper_thickness_;
  int num_of_nodes_;
  int num_of_elements_;
  int dimensions_of_x_;
  int dimensions_of_y_;
};


class currentsInHeater{
public:
  void InitilizecurrentsInHeater();
  void set_current_in_heater(const int the_heater_num, const double current_in_heater);
  std::vector<double>& get_current_in_heater() 
    {return currents_in_heater_;}

private:
  std::vector<double> currents_in_heater_;
};
void currentsInHeater::InitilizecurrentsInHeater(){
  currents_in_heater_.clear();
  currents_in_heater_.resize(Constants::kNumOfHeaters_,0.0);
}

void currentsInHeater::set_current_in_heater(const int the_heater_num, const double current_in_heater){
  currents_in_heater_[the_heater_num]=current_in_heater;
}


class ReadInput{
public:
  void ScanInputInformation();
  double get_time_to_turn_off_heaters() const 
    {return time_to_turn_off_heaters_;}
  int get_mesh_seeds_on_end() const 
    {return mesh_seeds_on_end_;}
  int get_mesh_seeds_on_gap() const 
    {return mesh_seeds_on_gap_;}
  int get_mesh_seeds_on_heater() const 
    {return mesh_seeds_on_heater_;}
  int get_mesh_seeds_along_isolater_thickness() const 
    {return mesh_seeds_along_isolater_thickness_;}
  int get_mesh_seeds_along_silicondioxide_thickness() const 
    {return mesh_seeds_along_silicondioxide_thickness_;}
  int get_mesh_seeds_along_copper_thickness() const 
    {return mesh_seeds_along_copper_thickness_;}
  int get_output_time_step_interval() const 
    {return output_time_step_interval_;}
  int get_maximum_time_steps() const 
    {return maximum_time_steps_;}
  double get_initial_time_increment() const 
    {return initial_time_increment_;}
  double get_minimum_time_increment() const 
    {return minimum_time_increment_;}
  double get_width_of_end() const 
    {return width_of_end_;}
  double get_thickness_of_copper() const 
    {return thickness_of_copper_;}
  double get_thickness_of_csilicon() const 
    {return thickness_of_csilicon_;}
  double get_thickness_of_isolater() const 
    {return thickness_of_isolater_;}
  double get_thickness_of_silicondioxide() const 
    {return thickness_of_silicondioxide_;}
  double get_thickness_of_titanium() const 
    {return thickness_of_titanium_;}
  double get_total_simulation_time() const 
    {return total_simulation_time_;}
  double get_width_of_heater() const 
    {return width_of_heater_;}
  double get_ambient_temperature() const 
    {return ambient_temperature_;}
  double get_boundary_condition_temperature() const 
    {return boundary_condition_temperature_;}
  double get_sample_initial_temperature() const 
    {return sample_initial_temperature_;}
  double get_maximum_temperature_change_per_time_increment() const 
    {return maximum_temperature_change_per_time_increment_;}
  std::vector<double>& get_current_in_heater()
    {return currents_in_heater_;}

private:
  double time_to_turn_off_heaters_;
  int mesh_seeds_on_end_;
  int mesh_seeds_on_gap_;
  int mesh_seeds_on_heater_;
  int mesh_seeds_along_isolater_thickness_;
  int mesh_seeds_along_silicondioxide_thickness_;
  int mesh_seeds_along_copper_thickness_;
  int output_time_step_interval_;
  int maximum_time_steps_;
  double initial_time_increment_;
  double minimum_time_increment_;
  double ambient_temperature_;
  double boundary_condition_temperature_;
  double width_of_end_;
  double sample_initial_temperature_;
  double thickness_of_copper_;
  double thickness_of_csilicon_;
  double thickness_of_isolater_;
  double thickness_of_silicondioxide_;
  double thickness_of_titanium_;
  double total_simulation_time_;
  double width_of_heater_;
  double maximum_temperature_change_per_time_increment_;
  std::vector<double> currents_in_heater_;
};
void ReadInput::ScanInputInformation(){
  std::ifstream ifs("input.txt", std::ios::in);
  char ignore_this_string[80];
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_on_end_;
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_on_gap_;
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_on_heater_;
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_along_isolater_thickness_;
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_along_silicondioxide_thickness_;
  ifs>>ignore_this_string;
  ifs>>mesh_seeds_along_copper_thickness_;
  ifs>>ignore_this_string;
  ifs>>time_to_turn_off_heaters_;
  ifs>>ignore_this_string;
  ifs>>output_time_step_interval_;
  ifs>>ignore_this_string;
  ifs>>maximum_time_steps_;
  ifs>>ignore_this_string;
  ifs>>total_simulation_time_;
  ifs>>ignore_this_string;
  ifs>>initial_time_increment_;
  ifs>>ignore_this_string;
  ifs>>minimum_time_increment_;
  ifs>>ignore_this_string;
  ifs>>maximum_temperature_change_per_time_increment_;
  ifs>>ignore_this_string;
  ifs>>width_of_end_;
  ifs>>ignore_this_string;
  ifs>>thickness_of_copper_;
  ifs>>ignore_this_string;
  ifs>>thickness_of_csilicon_;
  ifs>>ignore_this_string;
  ifs>>thickness_of_isolater_;
  ifs>>ignore_this_string;
  ifs>>thickness_of_silicondioxide_;
  ifs>>ignore_this_string;
  ifs>>thickness_of_titanium_;
  ifs>>ignore_this_string;
  ifs>>width_of_heater_;
  ifs>>ignore_this_string;
  ifs>>ambient_temperature_;
  ifs>>ignore_this_string;
  ifs>>boundary_condition_temperature_;
  ifs>>ignore_this_string;
  ifs>>sample_initial_temperature_;
  ifs>>ignore_this_string;

  currents_in_heater_.resize(Constants::kNumOfHeaters_, 0.0);
  for(int i=0;i<Constants::kNumOfHeaters_;i++){
    ifs>>currents_in_heater_[i]; 
//    ifs>>ignore_this_string; 
  }
  ifs.close();
}


// Class DisperseInputData disperse the data in Class ReadInput to the corresponding classified classes to store the data.
class Initialization{
public: 
  void ScanInputInformation()
    {read_input_.ScanInputInformation();}
  void DeliverDataToAnalysisConstants();
  void DeliverDataToMeshParameters();
  void DeliverDataToModelGeometry();
  void DeliverDataTocurrentInHeater();
  void InitializeInitialization();
  ModelGeometry *const get_model_geometry()  
    {return &model_geometry_;}
  AnalysisConstants *const get_analysis_constants() 
    {return &analysis_constants_;}
  MeshParameters *const get_mesh_parameters()  
    {return &mesh_parameters_;}
  currentsInHeater *const get_currents_in_heater() 
    {return &currents_in_heater_;}

protected:
  ModelGeometry model_geometry_;
  AnalysisConstants analysis_constants_;
  MeshParameters mesh_parameters_;
  currentsInHeater currents_in_heater_;
  ReadInput read_input_;
};
void Initialization::InitializeInitialization(){
  ScanInputInformation();
  DeliverDataToAnalysisConstants();
  DeliverDataToMeshParameters();
  DeliverDataToModelGeometry(); 
  currents_in_heater_.InitilizecurrentsInHeater();
  DeliverDataTocurrentInHeater(); 
  mesh_parameters_.set_dimensions_of_x();
  mesh_parameters_.set_dimensions_of_y();
  mesh_parameters_.set_num_of_nodes();
  mesh_parameters_.set_num_of_elements();
}

void Initialization::DeliverDataToAnalysisConstants(){
  analysis_constants_.set_time_to_turn_off_heaters(read_input_.get_time_to_turn_off_heaters());
  analysis_constants_.set_output_time_step_interval(read_input_.get_output_time_step_interval());
  analysis_constants_.set_maximum_time_steps(read_input_.get_maximum_time_steps());
  analysis_constants_.set_total_simulation_time(read_input_.get_total_simulation_time());
  analysis_constants_.set_initial_time_increment(read_input_.get_initial_time_increment());
  analysis_constants_.set_minimum_time_increment(read_input_.get_minimum_time_increment());
  analysis_constants_.set_maximum_temperature_change_per_time_increment(read_input_.get_maximum_temperature_change_per_time_increment());
  analysis_constants_.set_ambient_temperature(read_input_.get_ambient_temperature());
  analysis_constants_.set_boundary_condition_temperature(read_input_.get_boundary_condition_temperature());
  analysis_constants_.set_sample_initial_temperature(read_input_.get_sample_initial_temperature());
}

void Initialization::DeliverDataToMeshParameters(){
  mesh_parameters_.set_mesh_seeds_on_end(read_input_.get_mesh_seeds_on_end());
  mesh_parameters_.set_mesh_seeds_on_gap(read_input_.get_mesh_seeds_on_gap());
  mesh_parameters_.set_mesh_seeds_on_heater(read_input_.get_mesh_seeds_on_heater());
  mesh_parameters_.set_mesh_seeds_along_isolater_thickness(read_input_.get_mesh_seeds_along_isolater_thickness());
  mesh_parameters_.set_mesh_seeds_along_silicondioxide_thickness(read_input_.get_mesh_seeds_along_silicondioxide_thickness());
  mesh_parameters_.set_mesh_seeds_along_copper_thickness(read_input_.get_mesh_seeds_along_copper_thickness());
}

void Initialization::DeliverDataToModelGeometry(){
  model_geometry_.set_thickness_of_copper(read_input_.get_thickness_of_copper());
  model_geometry_.set_thickness_of_csilicon(read_input_.get_thickness_of_csilicon());
  model_geometry_.set_thickness_of_isolater(read_input_.get_thickness_of_isolater());
  model_geometry_.set_thickness_of_silicondioxide(read_input_.get_thickness_of_silicondioxide());
  model_geometry_.set_thickness_of_titanium(read_input_.get_thickness_of_titanium());
  model_geometry_.set_width_of_heater(read_input_.get_width_of_heater());
  model_geometry_.set_width_of_gap();
  model_geometry_.set_width_of_end(read_input_.get_width_of_end());
  model_geometry_.set_length_of_model();
  model_geometry_.set_new_width_of_end(read_input_.get_width_of_end());
}

void Initialization::DeliverDataTocurrentInHeater(){
  for(int i=0;i<Constants::kNumOfHeaters_;i++){
    currents_in_heater_.set_current_in_heater(i, read_input_.get_current_in_heater()[i]);
  }
}


//class GenerateMesh is used to generate the mesh for analysis
class GenerateMesh{
public:
  void CalculateCoordinates(Initialization* const);
  std::vector<double> &get_x_coordinates()
    {return x_coordinates_;}
  std::vector<double> &get_y_coordinates()
    {return y_coordinates_;}
  void GenerateMeshInitializeMeshSizeInfo(Initialization* const);
  void PrintCoordinatesResults(Initialization* const);

private:
  std::vector<double> x_coordinates_;
  std::vector<double> y_coordinates_;
  std::vector<double> x_coordinates_candidates;
  std::vector<double> y_coordinates_candidates;
  double mesh_size_on_end_;
  double mesh_size_on_gap_;
  double mesh_size_on_heater_;
  double mesh_size_along_isolater_thickness_;
  double mesh_size_along_silicondioxide_thickness_;
  double mesh_size_along_copper_thickness_;
};

void GenerateMesh::GenerateMeshInitializeMeshSizeInfo(Initialization *const initialization){
  mesh_size_on_end_ = (*((*initialization).get_model_geometry())).get_width_of_end()
                      /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end();
  mesh_size_on_gap_ = (*((*initialization).get_model_geometry())).get_width_of_gap()
                      /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap();
  mesh_size_on_heater_ = (*((*initialization).get_model_geometry())).get_width_of_heater()
                         /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater();
  mesh_size_along_isolater_thickness_ = (*((*initialization).get_model_geometry())).get_thickness_of_isolater()
                                              /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness();
  mesh_size_along_silicondioxide_thickness_ = (*((*initialization).get_model_geometry())).get_thickness_of_silicondioxide()
                                              /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_silicondioxide_thickness();
  mesh_size_along_copper_thickness_=(*((*initialization).get_model_geometry())).get_thickness_of_copper()
                                    /(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_copper_thickness();
}

void GenerateMesh::CalculateCoordinates(Initialization *const initialization){
  int num_of_elements_between_ends=Constants::kNumOfHeaters_*((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()
                                                              +(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater());
  int new_ordering_after_left_end;
  int check_modulues;
  int mesh_seeds_on_end;
  int num_of_nodes=(*((*initialization).get_mesh_parameters())).get_num_of_nodes();
  int num_of_elements=(*((*initialization).get_mesh_parameters())).get_num_of_elements();

  x_coordinates_.clear();
  x_coordinates_.resize(num_of_nodes,0.0);
  x_coordinates_candidates.clear();
  x_coordinates_candidates.resize((*((*initialization).get_mesh_parameters())).get_dimensions_of_x(),0.0);
  y_coordinates_.clear();
  y_coordinates_.resize(num_of_nodes,0.0);
  y_coordinates_candidates.clear();
  y_coordinates_candidates.resize((*((*initialization).get_mesh_parameters())).get_dimensions_of_y(),0.0);

  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_dimensions_of_x(); i++){

    if(i<=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end()){
      x_coordinates_candidates[i]=(mesh_size_on_end_*i);
    }

    else if(i>(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end() && 
            i<=((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end()
               +(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2)){
      x_coordinates_candidates[i] = x_coordinates_candidates[i-1] + mesh_size_on_gap_;
    }

    else if(i>((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end() 
             + (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2) && 
            i<=((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end() + num_of_elements_between_ends 
               -(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2)){
      new_ordering_after_left_end = i-(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end()
                                   -(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2-1;
      check_modulues = new_ordering_after_left_end%((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()
                      +(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap());

      if(check_modulues<(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()){
        x_coordinates_candidates[i] = x_coordinates_candidates[i-1]+mesh_size_on_heater_;
      }
      if(check_modulues>=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()){
        x_coordinates_candidates[i] = x_coordinates_candidates[i-1]+mesh_size_on_gap_;
      }
    }

    else if(i>((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end() + num_of_elements_between_ends 
           -(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2) &&
            i<=((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end() + num_of_elements_between_ends)){
      x_coordinates_candidates[i] = x_coordinates_candidates[i-1] + mesh_size_on_gap_;
    }

    else{
      x_coordinates_candidates[i] = x_coordinates_candidates[i-1] + mesh_size_on_end_;
    }
  }

  y_coordinates_candidates[0] = 0.0;
  y_coordinates_candidates[1] = 0.2;
  y_coordinates_candidates[2] = 0.4;
  y_coordinates_candidates[3] = 0.49;
  y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_-1] = 0.499;
  y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_]=(*((*initialization).get_model_geometry())).get_thickness_of_csilicon();

  int mesh_seeds_along_isolater_thickness=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness();
  for(int i=1; i<=mesh_seeds_along_isolater_thickness; i++){
    y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_+i]=(*((*initialization).get_model_geometry())).get_thickness_of_csilicon()
                                                                           + i*mesh_size_along_isolater_thickness_;
  }

  y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_+mesh_seeds_along_isolater_thickness+1]=(*((*initialization).get_model_geometry())).get_thickness_of_csilicon()
                                     +(*((*initialization).get_model_geometry())).get_thickness_of_isolater()+(*((*initialization).get_model_geometry())).get_thickness_of_titanium();

  for(int i=1; i<=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_silicondioxide_thickness(); i++){
    y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_+mesh_seeds_along_isolater_thickness+1+i]=
                                        (*((*initialization).get_model_geometry())).get_thickness_of_csilicon() 
                                       +(*((*initialization).get_model_geometry())).get_thickness_of_isolater() 
                                       +(*((*initialization).get_model_geometry())).get_thickness_of_titanium() 
                                       + i*mesh_size_along_silicondioxide_thickness_;
  }

  for(int i=1; i<=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_copper_thickness(); i++){
    y_coordinates_candidates[Constants::kMeshSeedsAlongSiliconThickness_+mesh_seeds_along_isolater_thickness+1
                            +(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_silicondioxide_thickness()+i] 
      = (*((*initialization).get_model_geometry())).get_thickness_of_csilicon() 
      + (*((*initialization).get_model_geometry())).get_thickness_of_isolater() 
      + (*((*initialization).get_model_geometry())).get_thickness_of_titanium() 
      + (*((*initialization).get_model_geometry())).get_thickness_of_silicondioxide() + i*mesh_size_along_copper_thickness_;
  }

  //generate coordinates for nodes   
  for(int j=0; j<(*((*initialization).get_mesh_parameters())).get_dimensions_of_y(); j++){
    for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_dimensions_of_x(); i++){
      x_coordinates_[(*((*initialization).get_mesh_parameters())).get_dimensions_of_x()*j+i]=x_coordinates_candidates[i];
      y_coordinates_[(*((*initialization).get_mesh_parameters())).get_dimensions_of_x()*j+i]=y_coordinates_candidates[j];
    }
  }
//  printf("Generating mesh information completed\n");
}

void GenerateMesh::PrintCoordinatesResults(Initialization *const initialization){  
  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_num_of_nodes(); i++)
    printf("x_coordinates_[%d] = %f  y_coordinates_[%d] = %f\n", i, x_coordinates_[i], i, y_coordinates_[i]);
}


class DegreeOfFreedomAndEquationNumbers{
public:
  void InitializeDegreeOfFreedomAndEquationNumbers(Initialization *const);
  std::vector<int> &get_nodes_in_elements()
    {return nodes_in_elements_;}
  std::vector<int> &get_equation_numbers_of_nodes()
    {return equation_numbers_of_nodes_;}
  std::vector<int> &get_equation_numbers_in_elements()
    {return equation_numbers_in_elements_;}
  std::vector<int> &get_essential_bc_nodes()
    {return essential_bc_nodes_;}
  double get_num_of_essential_bc_nodes() const
    {return num_of_essential_bc_nodes_;}
  int get_num_of_equations() const
    {return num_of_equations_;}
  void set_essential_bc_nodes(Initialization* const, std::vector<double>&);
  void GenerateNodeAndEquationNumbersInElements(Initialization*);
  void PrintDofAndEquationNumbers(Initialization *const);

private:
  std::vector<int> nodes_in_elements_;
  std::vector<int> essential_bc_nodes_;
  int num_of_essential_bc_nodes_;
  std::vector<int> equation_numbers_of_nodes_;
  std::vector<int> equation_numbers_in_elements_;
  int num_of_equations_;
};
void DegreeOfFreedomAndEquationNumbers::InitializeDegreeOfFreedomAndEquationNumbers(Initialization *const initialization){
  num_of_essential_bc_nodes_=(*((*initialization).get_mesh_parameters())).get_dimensions_of_x();
}

void DegreeOfFreedomAndEquationNumbers::set_essential_bc_nodes(Initialization *const initialization, std::vector<double> &y_coordinates){
  essential_bc_nodes_.clear();
  essential_bc_nodes_.resize(num_of_essential_bc_nodes_, 0);
  int count_current_node_number=0;
  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_num_of_nodes(); i++){
    if(y_coordinates[i]==Constants::kMinYCoordinate_){
      if(count_current_node_number>=num_of_essential_bc_nodes_){
        printf("number of nodes of essential boundary condition exceeds the right value\n");
        exit(-1);
      }
      essential_bc_nodes_[count_current_node_number]=i;
      ++count_current_node_number;
    }
  }
}

void DegreeOfFreedomAndEquationNumbers::GenerateNodeAndEquationNumbersInElements(Initialization *const initialization){
//generate node numbering for each element
  int num_of_elements=(*((*initialization).get_mesh_parameters())).get_num_of_elements();
  int num_of_nodes=(*((*initialization).get_mesh_parameters())).get_num_of_nodes();

  nodes_in_elements_.clear();
  nodes_in_elements_.resize(Constants::kNumOfNodesInElement_*num_of_elements,0);
  for(int j=0; j<((*((*initialization).get_mesh_parameters())).get_dimensions_of_y()-1); j++){  
    for(int i=0; i<((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1); i++){
      int temporary_node_id[4]={0,0,0,0};
      int element_number = j*((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1) + i; 
      temporary_node_id[0] = ((j*(*((*initialization).get_mesh_parameters())).get_dimensions_of_x())+i); 
      temporary_node_id[1] = temporary_node_id[0]+1;   
      temporary_node_id[2] = (((j+1)*(*((*initialization).get_mesh_parameters())).get_dimensions_of_x()) + i + 1); 
      temporary_node_id[3] = temporary_node_id[2]-1; 
      nodes_in_elements_[0+element_number*Constants::kNumOfNodesInElement_] = temporary_node_id[0];
      nodes_in_elements_[1+element_number*Constants::kNumOfNodesInElement_] = temporary_node_id[1];
      nodes_in_elements_[2+element_number*Constants::kNumOfNodesInElement_] = temporary_node_id[2];
      nodes_in_elements_[3+element_number*Constants::kNumOfNodesInElement_] = temporary_node_id[3];
    }
  }

  equation_numbers_of_nodes_.clear();
  equation_numbers_of_nodes_.resize(num_of_nodes,0);
  int count_equation_number=0;
  for(int i=0; i<num_of_nodes; i++){
    int count_node=0;
    bool check_fixed_dof = false;
    while(count_node<num_of_essential_bc_nodes_){ 
      if(i==essential_bc_nodes_[count_node]){
        check_fixed_dof = true;
        break; //find the DOF K that has not been fixed
      }
      count_node++;
    }
    if(check_fixed_dof == false){
      equation_numbers_of_nodes_[i]=count_equation_number;
      count_equation_number++;
    }
    else if(check_fixed_dof == true){
      equation_numbers_of_nodes_[i]=-1;
    }
  }    
      
  equation_numbers_in_elements_.clear();
  equation_numbers_in_elements_.resize(Constants::kNumOfNodesInElement_*num_of_elements,0);
  for(int i=0; i<num_of_elements; i++){
    for(int j=0; j<Constants::kNumOfNodesInElement_; j++){  
      int current_node_number=nodes_in_elements_[j+i*Constants::kNumOfNodesInElement_];
      equation_numbers_in_elements_[j+i*Constants::kNumOfNodesInElement_]=equation_numbers_of_nodes_[current_node_number];
    }
  }

  num_of_equations_=equation_numbers_of_nodes_.back()+1;
}

void DegreeOfFreedomAndEquationNumbers::PrintDofAndEquationNumbers(Initialization *const initialization){
  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_num_of_nodes(); i++)
    printf("equation_numbers_of_nodes_[%d]=%d\n", i, equation_numbers_of_nodes_[i]);
  printf("\n\nequation_numbers_in_elements_\n");
  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_num_of_elements(); i++){
    for(int j=0; j<Constants::kNumOfNodesInElement_; j++)
      printf("%d  ", equation_numbers_in_elements_[j+i*Constants::kNumOfNodesInElement_]);
    printf("\n");
  }
}

class HalfBandWidth{
public:
  void set_accumulative_half_band_width_vector(Initialization *const, DegreeOfFreedomAndEquationNumbers *);
  std::vector<int>& get_accumulative_half_band_width_vector(){return accumulative_half_band_width_vector_;}
  double get_size_of_desparsed_stiffness_matrix(){return size_of_desparsed_stiffness_matrix_;}
  void PrintHalfBandWidthInformation(int num_of_equations){
    for(int i=0;i<num_of_equations;i++)
      printf("accumulative half band-width at [%d] column/row is %d\n",i,accumulative_half_band_width_vector_[i]);
  }

private:
  std::vector<int> accumulative_half_band_width_vector_;
  int size_of_desparsed_stiffness_matrix_;
};
void HalfBandWidth::set_accumulative_half_band_width_vector(Initialization *const initialization, DegreeOfFreedomAndEquationNumbers *const dof_and_equation_numbers){
  int num_of_equations = (*dof_and_equation_numbers).get_num_of_equations();
  int num_of_elements = (*((*initialization).get_mesh_parameters())).get_num_of_elements();
  std::vector<int>& equation_numbers_of_nodes=(*dof_and_equation_numbers).get_equation_numbers_of_nodes();
  std::vector<int> equation_numbers_in_elements=(*dof_and_equation_numbers).get_equation_numbers_in_elements();

  accumulative_half_band_width_vector_.resize(num_of_equations,0);
  int smallest_equation_number;
  for(int i=0;i<num_of_equations;i++){
    smallest_equation_number=num_of_equations-1;
    for(int j=0;j<num_of_elements;j++){//searching...
      for(int k=0;k<Constants::kNumOfNodesInElement_;k++){ //searching...
        if(equation_numbers_in_elements[k+Constants::kNumOfNodesInElement_*j]==i){
          for(int l=0;l<Constants::kNumOfNodesInElement_;l++){
            if(equation_numbers_in_elements[l+Constants::kNumOfNodesInElement_*j]<smallest_equation_number && 
               equation_numbers_in_elements[l+Constants::kNumOfNodesInElement_*j]>=0) 
              smallest_equation_number=equation_numbers_in_elements[l+Constants::kNumOfNodesInElement_*j];
          }
        }
      }  
    }
    int half_band_width=(i-smallest_equation_number)+1; // half band width, gap+1
    if(i!=0) accumulative_half_band_width_vector_[i]=accumulative_half_band_width_vector_[i-1]+half_band_width;
  }
  size_of_desparsed_stiffness_matrix_ = accumulative_half_band_width_vector_[num_of_equations-1]+1;
}



// find boundary nodes
class BoundaryCondition{
public:
  void InitializeBoundaryCondition(Initialization *const);
  void FixTemperature(int, std::vector<std::vector<double> >&, std::vector<int>&, std::vector<double>&);
  void PrintBoundaryConditionNodes(std::vector<int>& essential_bc_nodes){
    int num_of_essential_bc_nodes = essential_bc_nodes.size();
    for(int i=0;i<num_of_essential_bc_nodes;i++)
      printf("essential_bc_nodes_[%d] = %d\n", i, essential_bc_nodes[i]);
    printf("boundary_condition_temperature_ = %f\n", boundary_condition_temperature_);
  }

private:
  double boundary_condition_temperature_;
};
void BoundaryCondition::InitializeBoundaryCondition(Initialization *const initialization){
  boundary_condition_temperature_=(*((*initialization).get_analysis_constants())).get_boundary_condition_temperature();
}

void BoundaryCondition::FixTemperature(const int element_number, std::vector<std::vector<double> >&element_stiffness_matrix, std::vector<int>&equation_numbers_in_elements, std::vector<double>&heat_load){
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    int row_equation_number=equation_numbers_in_elements[k+Constants::kNumOfNodesInElement_*element_number]; 
    for(int jj=0;jj<Constants::kNumOfNodesInElement_;jj++){//loop over all force components in this element
      int column_equation_number=equation_numbers_in_elements[jj+Constants::kNumOfNodesInElement_*element_number]; 
      if(row_equation_number>=0 && column_equation_number<0){
        heat_load[row_equation_number] += -(element_stiffness_matrix[k][jj]*boundary_condition_temperature_);
      }
    }//for
  }//for       
//  printf("Processing fixed temperature boundary condition completed\n");
} 


class MappingShapeFunctionAndDerivatives{
public:
  void InitializeMappingShapeFunctionAndDerivatives();
  void set_coordinates_in_this_element(int, std::vector<int>&, std::vector<double>&, std::vector<double>&);
  void set_shape_function(double, double);
  void set_shape_function_derivatives(double, double);
  void set_determinant_of_jacobian_matrix();
  void set_dn_dx();
  void PrintDeterminantOfJacobianMatrix();

protected:
  std::vector<std::vector<double> > coordinates_in_this_element_;
  std::vector<double> shape_function_;
  std::vector<std::vector<double> > shape_function_derivatives_;
  std::vector<std::vector<double> > dn_dx_;
  double determinant_of_jacobian_matrix_;
  double jacobian_matrix_[2][2];
};
void MappingShapeFunctionAndDerivatives::InitializeMappingShapeFunctionAndDerivatives(){
  coordinates_in_this_element_.resize(2);
  coordinates_in_this_element_[0].resize(Constants::kNumOfNodesInElement_, 0.0);
  coordinates_in_this_element_[1].resize(Constants::kNumOfNodesInElement_, 0.0);
  
  shape_function_.resize(Constants::kNumOfNodesInElement_, 0.0);
   
  shape_function_derivatives_.resize(2);
  shape_function_derivatives_[0].resize(Constants::kNumOfNodesInElement_, 0.0);
  shape_function_derivatives_[1].resize(Constants::kNumOfNodesInElement_, 0.0);

  dn_dx_.resize(2);
  dn_dx_[0].resize(Constants::kNumOfNodesInElement_, 0.0);
  dn_dx_[1].resize(Constants::kNumOfNodesInElement_, 0.0);
}

void MappingShapeFunctionAndDerivatives::set_coordinates_in_this_element(const int element_number, std::vector<int>& nodes_in_elements,
std::vector<double>& x_coordinates, std::vector<double>& y_coordinates){
  for (int k=0;k<Constants::kNumOfNodesInElement_;k++){
    coordinates_in_this_element_[0][k]=x_coordinates[nodes_in_elements[k+element_number*Constants::kNumOfNodesInElement_]];
    coordinates_in_this_element_[1][k]=y_coordinates[nodes_in_elements[k+element_number*Constants::kNumOfNodesInElement_]]; 
  }
}

void MappingShapeFunctionAndDerivatives::set_shape_function(const double ksi_coordinate, const double eta_coordinate){
  shape_function_[0] = 0.25*(1-ksi_coordinate)*(1-eta_coordinate);
  shape_function_[1] = 0.25*(1+ksi_coordinate)*(1-eta_coordinate);
  shape_function_[2] = 0.25*(1+ksi_coordinate)*(1+eta_coordinate);
  shape_function_[3] = 0.25*(1-ksi_coordinate)*(1+eta_coordinate);
}

void MappingShapeFunctionAndDerivatives::set_shape_function_derivatives(const double ksi_coordinate, const double eta_coordinate){
  //----------------------to get jacobian determinant----------------------
  // Calculate the local derivatives of the shape functions.
  shape_function_derivatives_[0][0] = -0.25*(1 - eta_coordinate);
  shape_function_derivatives_[1][0] = -0.25*(1 - ksi_coordinate);
  shape_function_derivatives_[0][1] =  0.25*(1 - eta_coordinate);
  shape_function_derivatives_[1][1] = -0.25*(1 + ksi_coordinate);
  shape_function_derivatives_[0][2] =  0.25*(1 + eta_coordinate);
  shape_function_derivatives_[1][2] =  0.25*(1 + ksi_coordinate);
  shape_function_derivatives_[0][3] = -0.25*(1 + eta_coordinate);
  shape_function_derivatives_[1][3] =  0.25*(1 - ksi_coordinate);
}

void MappingShapeFunctionAndDerivatives::set_determinant_of_jacobian_matrix(){
  for(int u=0;u<2;u++){
   for(int v=0;v<2;v++){
      determinant_of_jacobian_matrix_=0.0;
      for(int m=0;m<Constants::kNumOfNodesInElement_;m++){
        determinant_of_jacobian_matrix_ += shape_function_derivatives_[u][m]*coordinates_in_this_element_[v][m];
      }
      jacobian_matrix_[u][v]=determinant_of_jacobian_matrix_;
    } 
  }
  determinant_of_jacobian_matrix_=jacobian_matrix_[0][0]*jacobian_matrix_[1][1]-jacobian_matrix_[0][1]*jacobian_matrix_[1][0];
  determinant_of_jacobian_matrix_=fabs(determinant_of_jacobian_matrix_);
}
  
void MappingShapeFunctionAndDerivatives::set_dn_dx(){
  //-------------Jacobian inverse----------------------
  double reciprocal=1.0/determinant_of_jacobian_matrix_;
  double inverse_of_jacobian[2][2];
  inverse_of_jacobian[0][0]=reciprocal*jacobian_matrix_[1][1];
  inverse_of_jacobian[1][1]=reciprocal*jacobian_matrix_[0][0];
  inverse_of_jacobian[0][1]=-reciprocal*jacobian_matrix_[0][1];
  inverse_of_jacobian[1][0]=-reciprocal*jacobian_matrix_[1][0];
  //------------Derivatives of shape function w.r.t. global coordinate--------------
  for(int u=0;u<Constants::kNumOfNodesInElement_;u++){
    for(int v=0;v<2;v++){
      dn_dx_[v][u]=0.0;
      for(int m=0;m<2;m++){
        dn_dx_[v][u] += inverse_of_jacobian[m][v]*shape_function_derivatives_[m][u];
      }
    }
  }
}

void MappingShapeFunctionAndDerivatives::PrintDeterminantOfJacobianMatrix(){
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("coordinates_in_this_element_[0][%d] = %f\n", k, coordinates_in_this_element_[0][k]);
    printf("coordinates_in_this_element_[1][%d] = %f\n", k, coordinates_in_this_element_[1][k]);
  }
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("shape_function_[%d] = %f\n", k, shape_function_[k]);
  }
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("shape_function_derivatives_[0][%d] = %f\n", k, shape_function_derivatives_[0][k]);
    printf("shape_function_derivatives_[1][%d] = %f\n", k, shape_function_derivatives_[1][k]);
  }
  printf("determinant_of_jacobian_matrix_ = %f\n", determinant_of_jacobian_matrix_);
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("dn_dx_[0][%d] = %f\n", k, dn_dx_[0][k]);
    printf("dn_dx_[1][%d] = %f\n", k, dn_dx_[1][k]);
  }
}


class TemperatureDependentVariables{
public:
  void InitializeTemperatureDependentVariables(Initialization *const);
  double get_thermal_conductivity(int, double, std::vector<int>&);
  double get_thermal_conductivity_derivative(int, double, std::vector<int>&);
  double get_body_heat_flux(double, double);
  double get_body_heat_flux_derivative(double, double);
  double get_heater_crosssection_area() const
    {return heater_crosssection_area_mm_square_;}
  double get_specific_heat(int, double, std::vector<int>&);
  double get_emissivity(double); 
  double get_emissivity_derivative(double);
  double get_heater_crosssection_area_mm_square() const
    {return heater_crosssection_area_mm_square_;}
  void PrintTemperatureDependentVariables(std::vector<int>&,Initialization *const);
 
private:
  double heater_crosssection_area_mm_square_;
};
void TemperatureDependentVariables::InitializeTemperatureDependentVariables(Initialization *const initialization){
  heater_crosssection_area_mm_square_=(*((*initialization).get_model_geometry())).get_thickness_of_titanium()
                                      *(*((*initialization).get_model_geometry())).get_width_of_heater();
}

double TemperatureDependentVariables::get_thermal_conductivity(const int element_number, const double temperature, std::vector<int>& material_id_of_elements){
  double conductivity;
  int material_id=material_id_of_elements[element_number];
  switch(material_id){
    case 0: conductivity=0.0002723*temperature*temperature-0.5435*temperature+295.9; break; //csilicon
    case 1: conductivity=1.375e-05*temperature*temperature-0.01653*temperature+22.72; break;  //titanium
    case 2: conductivity=4.8e-05*temperature*temperature-0.06094*temperature+24.24; break; //silicondioxide
    case 3: conductivity=-9.085e-06*temperature*temperature-0.05699*temperature+405.6987; break; // copper
  }
  return conductivity;
}

double TemperatureDependentVariables::get_thermal_conductivity_derivative(const int element_number, const double temperature, std::vector<int>& material_id_of_elements){
  double conductivity_derivative;
  int material_id=material_id_of_elements[element_number];
  switch(material_id){
    case 0: conductivity_derivative=0.0002723*2*temperature-0.5435; break; 
    case 1: conductivity_derivative=1.375e-05*2*temperature-0.01653; break;
    case 2: conductivity_derivative=4.8e-05*2*temperature-0.06094; break; 
    case 3: conductivity_derivative=-9.085e-06*2*temperature-0.05699; break;
  }
  return conductivity_derivative;
}

double TemperatureDependentVariables::get_specific_heat(const int element_number, const double temperature, std::vector<int>& material_id_of_elements){ //Cal/g/K = 4.184e9 mJ/tonne/K
  double specific_heat;
  int material_id=material_id_of_elements[element_number];
  switch(material_id){
    case 0: specific_heat=-632.6*temperature*temperature+9.952e+5*temperature+5.189e+8; break;//csilicon
    case 1: specific_heat=714.2*temperature*temperature-6.233e+5*temperature+7.128e+8; break;//titanium
    case 2: specific_heat=-437.4*temperature*temperature+1.404e+6*temperature+3.903e+8; break; //silicondioxide
    case 3: specific_heat=180.2*temperature*temperature-1.723e+5*temperature+4.175e+8; break;// copper
  }
  return specific_heat;
}

double TemperatureDependentVariables::get_emissivity(const double temperature){ //Cal/g/K = 4.184e9 mJ/tonne/K    
//data from curve 52 P148
  double emissivity;
  emissivity=-1.932e-7*temperature*temperature+0.0003696*temperature+0.07681;// copper
  return emissivity;
}

double TemperatureDependentVariables::get_emissivity_derivative(const double temperature){ //Cal/g/K = 4.184e9 mJ/tonne/K    
//data from curve 52 P148
  double emissivity_derivative;
  emissivity_derivative=-1.932e-7*2*temperature+0.0003696;// copper
  return emissivity_derivative;
}


double TemperatureDependentVariables::get_body_heat_flux(const double temperature, const double current_in_element_ma){ //Cal/g/K = 4.184e9 mJ/tonne/K    
//data from 
//Bel’skaya, E. A. "An experimental investigation of the electrical resistivity of titanium in the temperature range from 77 to 1600 K." High Temperature 43.4 (2005): 546-553.
//---------------------
  double body_heat_flux;
  double resistivity;
  double current_in_element_a = current_in_element_ma*1.0e-3;  // mA -> A
  double heater_crosssection_area_m_square=heater_crosssection_area_mm_square_*1.0e-3*1.0e-3;  //mm^2->m^2
  resistivity = 1.403e-15*temperature*temperature + 1.037e-8*temperature - 2.831e-6;// titanium heater
  body_heat_flux = resistivity*pow((current_in_element_a/heater_crosssection_area_m_square),2);
  body_heat_flux *= 1.0e-6;
  return body_heat_flux;
}

double TemperatureDependentVariables::get_body_heat_flux_derivative(const double temperature, const double current_in_element_ma){ //Cal/g/K = 4.184e9 mJ/tonne/K    
//data from 
//Bel’skaya, E. A. "An experimental investigation of the electrical resistivity of titanium in the temperature range from 77 to 1600 K." High Temperature 43.4 (2005): 546-553.
//---------------------
  double resistivity_derivative;
  double body_heat_flux_derivative;
  double current_in_element_a=current_in_element_ma*1.0e-3;
  double heater_crosssection_area_m_square=heater_crosssection_area_mm_square_*1.0e-3*1.0e-3;  //mm^2->m^2
  resistivity_derivative=1.403e-15*2*temperature+1.037e-8;// titanium heater
  body_heat_flux_derivative = resistivity_derivative*pow((current_in_element_a/heater_crosssection_area_m_square),2);
  body_heat_flux_derivative *= 1.0e-6;
  return body_heat_flux_derivative;
}

void TemperatureDependentVariables::PrintTemperatureDependentVariables(std::vector<int>& material_id_of_elements, Initialization *const initialization){
  for(int i=0;i<Constants::kNumOfHeaters_;i++){
    printf("currents_in_heater_[%d] = %f\n", i, (*((*initialization).get_currents_in_heater())).get_current_in_heater()[i]);
  }
  printf("get_heater_crosssection_area = %f\n", heater_crosssection_area_mm_square_);
  printf("get_thermal_conductivity = %f\n", get_thermal_conductivity(1, 300.0, material_id_of_elements));
  printf("get_thermal_conductivity_derivative = %f\n", get_thermal_conductivity_derivative(1, 300.0, material_id_of_elements));
  printf("get_specific_heat = %f\n", get_specific_heat(1, 300.0, material_id_of_elements));
  printf("get_emissivity = %f\n", get_emissivity(300.0));
  printf("get_emissivity_derivative = %f\n", get_emissivity_derivative(300.0));
  printf("get_body_heat_flux = %f\n", get_body_heat_flux(300.0, 50.0));
  printf("get_body_heat_flux_derivative = %.16f\n", get_body_heat_flux_derivative(300.0, 50.0)); 
}


class HeaterElements:public MappingShapeFunctionAndDerivatives {
public:
  void InitializeHeaterElements(Initialization *const);
  void set_elements_as_heater(Initialization *const);
  double get_num_of_elements_as_heater() const
    {return num_of_elements_as_heater_;}
  std::vector<int> &get_elements_as_heater() 
    {return elements_as_heater_;}
  void HeatSupply(int, int, std::vector<double>&, std::vector<int>&,std::vector<int>&, std::vector<double>&,   
  TemperatureDependentVariables*,Initialization *const);
  void PrintHeaterElements(){
    for(int i=0;i<num_of_elements_as_heater_;i++)
      printf("elements_as_heater_[%d] = %d\n", i, elements_as_heater_[i]);
  }

private:
  int num_of_elements_as_heater_;
  std::vector<int> elements_as_heater_;
};
void HeaterElements::InitializeHeaterElements(Initialization *const initialization){
  num_of_elements_as_heater_ = (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()*Constants::kNumOfHeaters_;
  elements_as_heater_.resize(num_of_elements_as_heater_, 0);
  InitializeMappingShapeFunctionAndDerivatives();
}

void HeaterElements::set_elements_as_heater(Initialization *const initialization){
  int dimensions_of_x = (*((*initialization).get_mesh_parameters())).get_dimensions_of_x();
  int mesh_seeds_on_end = (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end();
  int mesh_seeds_on_gap = (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap();
  int mesh_seeds_on_heater = (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater();
  int first_heater_element_=(Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness())
                            *(dimensions_of_x-1)+mesh_seeds_on_end+mesh_seeds_on_gap/2;
  for(int i=0; i<Constants::kNumOfHeaters_; i++){
    for(int j=0; j<mesh_seeds_on_heater; j++){  
      elements_as_heater_[i*mesh_seeds_on_heater+j] = first_heater_element_ + i*(mesh_seeds_on_gap+mesh_seeds_on_heater) + j;                 
    }
  }
}

void HeaterElements::HeatSupply(const int element_number, const int heater_element_number, std::vector<double>&heat_load, std::vector<int>&nodes_in_elements, std::vector<int>&equation_numbers_in_elements, std::vector<double>&current_temperature_field, TemperatureDependentVariables *const temperature_dependent_variables,Initialization *const initialization){//Calculate internal load contribution      
  //integration rule
  int num_of_integration_points=2;
  double coordinates_of_integration_points[2]={-0.57735026, 0.57735026}; //gaussian quadrature coordinates
  double weights_of_integration_points[2]={1.0, 1.0};//weight of gaussian point
  
  int heater_number=heater_element_number/(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater();
  double current=(*((*initialization).get_currents_in_heater())).get_current_in_heater()[heater_number];

  for(int k=0;k<num_of_integration_points;k++){
    double ksi_coordinate=coordinates_of_integration_points[k];  //gaussian piont coordinate
    double ksi_weight=weights_of_integration_points[k];    //weight of gaussian quadrature
    for(int l=0;l<num_of_integration_points;l++){
      double eta_coordinate=coordinates_of_integration_points[l]; 
      double eta_weight=weights_of_integration_points[l];  

      set_shape_function(ksi_coordinate, eta_coordinate);
      set_shape_function_derivatives(ksi_coordinate, eta_coordinate);
      set_determinant_of_jacobian_matrix();  

      double temperature=0.0;
      for(int ii=0;ii<Constants::kNumOfNodesInElement_;ii++)
        temperature += current_temperature_field[(nodes_in_elements[ii+element_number*4])]*shape_function_[ii];

      for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
        double equation_number=equation_numbers_in_elements[j+element_number*4];
        if(equation_number>=0){
          heat_load[equation_number] += shape_function_[j]*
          (*temperature_dependent_variables).get_body_heat_flux(temperature,current)*determinant_of_jacobian_matrix_*ksi_weight*eta_weight;
        }//if
      }//for j
    }//for l
  }//for k
//  printf("processing heat supply element completed\n");
}

// find radiation elements
class RadiationElements{
public:
  void InitializeRadiationElements(Initialization *const);
  void set_elements_with_radiation(Initialization *const);
  double get_num_of_elements_with_radiation() const
    {return num_of_elements_with_radiation_;}
  std::vector<int> &get_elements_with_radiation() 
    {return elements_with_radiation_;}
  void PrintRadiationElements();

private:
  int num_of_elements_with_radiation_;
  std::vector<int> elements_with_radiation_;
};
void RadiationElements::InitializeRadiationElements(Initialization *const initialization){
  num_of_elements_with_radiation_=(*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1;
}

void RadiationElements::set_elements_with_radiation(Initialization *const initialization){
  elements_with_radiation_.resize(num_of_elements_with_radiation_, 0);
  for(int i=0; i<num_of_elements_with_radiation_; i++){
    elements_with_radiation_[i] = ((*((*initialization).get_mesh_parameters())).get_dimensions_of_y()-2)
                                 *num_of_elements_with_radiation_+ i;                 
  }
}

void RadiationElements::PrintRadiationElements(){
  for(int i=0;i<num_of_elements_with_radiation_;i++){
    printf("elements_with_radiation_[%d] = %d\n", i, elements_with_radiation_[i]);
  }
}


class TemperatureFieldInitial{
public:
  void set_initial_temperature_field(std::vector<int>&, std::vector<double>&, Initialization *const, std::vector<int>&);
  void PrintInitialTemperatureField(std::vector<double>&);
};
void TemperatureFieldInitial::set_initial_temperature_field(std::vector<int>&essential_bc_nodes, std::vector<double>&initial_temperature_field,Initialization *const initialization, std::vector<int>& equation_numbers_of_nodes){
  double boundary_condition_temperature_=(*((*initialization).get_analysis_constants())).get_boundary_condition_temperature();
  double sample_initial_temperature_=(*((*initialization).get_analysis_constants())).get_sample_initial_temperature();
  int num_of_nodes = (*((*initialization).get_mesh_parameters())).get_num_of_nodes();
  for(int i=0; i<num_of_nodes; i++){
    if(equation_numbers_of_nodes[i]<0) initial_temperature_field[i]=boundary_condition_temperature_;
    else initial_temperature_field[i]=sample_initial_temperature_;
  }
}

void TemperatureFieldInitial::PrintInitialTemperatureField(std::vector<double>&initial_temperature_field){
  for(int i=0;i<initial_temperature_field.size();i++){
    printf("initial_temperature_field[%d] = %f\n", i, initial_temperature_field[i]);
  }
}


class MaterialParameters{
public:
  void set_material_id_of_elements(Initialization *const);
  void set_densities();
  std::vector<int>& get_material_id_of_elements(){return material_id_of_elements_;}
  std::vector<double>& get_densities(){return densities_;}
  void PrintMaterialParameters();

private:
  std::vector<double> densities_;
  std::vector<int> material_id_of_elements_;
};
void MaterialParameters::set_densities(){
  densities_.resize(Constants::kNumOfMaterials_,0.0);
  //material information density [tonne]/[mm]^3
  densities_[0] = 2.329e-9;  // csilicon
  densities_[1] = 4.506e-9;  // titanium (heater)
  densities_[2] = 2.65e-9; // silicon dioxide
  densities_[3] = 8.96e-9; // copper
}

void MaterialParameters::set_material_id_of_elements(Initialization *const initialization){
  material_id_of_elements_.resize((*((*initialization).get_mesh_parameters())).get_num_of_elements(),0);
  int first_element_of_silicon = 0;
  int last_element_of_silicon = Constants::kMeshSeedsAlongSiliconThickness_
                               *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1)-1;
  //isolater film
  int first_element_of_isolater=Constants::kMeshSeedsAlongSiliconThickness_
                               *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1);
  int last_element_of_isolater=(Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness())
                               *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1)-1;

  //heater and silicon dioxide mix layer (ends excluded!!!)
  int first_element_of_heater_gap_mixed = (Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness())
                                         *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1)
                                        + (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end();
  int last_element_of_heater_gap_mixed = (Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness()+1)
                                        *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1)-1
                                        - (*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_end();
  int first_element_of_pure_layer_of_silicondioxide = (Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness()+1)
                                                     *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1);
  int last_element_of_pure_layer_of_silicondioxide = (Constants::kMeshSeedsAlongSiliconThickness_+(*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_isolater_thickness()+1
                                                   + (*((*initialization).get_mesh_parameters())).get_mesh_seeds_along_silicondioxide_thickness())
                                                     *((*((*initialization).get_mesh_parameters())).get_dimensions_of_x()-1)-1;
  int first_element_of_copper = last_element_of_pure_layer_of_silicondioxide+1;   
  int last_element_of_copper = (*((*initialization).get_mesh_parameters())).get_num_of_elements()-1;

  for(int i=0; i<(*((*initialization).get_mesh_parameters())).get_num_of_elements(); i++){
    //csilicon
    if(i<=last_element_of_silicon){
      material_id_of_elements_[i]=0;
    }
    
    // isolater layer
    else if(i>=first_element_of_isolater && i<=last_element_of_isolater){
      material_id_of_elements_[i]=2;
    }

    // left end on the mix layer
    else if(i>last_element_of_silicon && i<first_element_of_heater_gap_mixed){
      material_id_of_elements_[i]=2;
    }

    //heater and silicon dioxide mixed layer
    else if(i>=first_element_of_heater_gap_mixed && i<=last_element_of_heater_gap_mixed){
      if( i-first_element_of_heater_gap_mixed>=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2 && 
          last_element_of_heater_gap_mixed-i>=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2 ){

        int new_label_start_from_zero = i-first_element_of_heater_gap_mixed-(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()/2;
        int check_gap_heater_belonging = (new_label_start_from_zero%((*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_gap()
                                                                    +(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()));
        if(check_gap_heater_belonging<(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()) 
          material_id_of_elements_[i]=1;
        if(check_gap_heater_belonging>=(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater()) 
          material_id_of_elements_[i]=2;
      }
      else material_id_of_elements_[i]=2; //Elements belong to gap
    }

    // right end on the mix layer
    else if(i>last_element_of_heater_gap_mixed && i<first_element_of_pure_layer_of_silicondioxide){
      material_id_of_elements_[i]=2;
    }

    //silicon dioxide 
    else if(i>=first_element_of_pure_layer_of_silicondioxide && i<=last_element_of_pure_layer_of_silicondioxide){
      material_id_of_elements_[i]=2;
    }

    //copper
    else if(i>=first_element_of_copper && i<=last_element_of_copper){
      material_id_of_elements_[i]=3;
    }
    
    else if(i>last_element_of_copper){
      printf("Asigning material id to elements is wrong\n");
      exit(-1);
    }
  }
}

void MaterialParameters::PrintMaterialParameters(){
  for(int i=0;i<Constants::kNumOfMaterials_;i++){
    printf("densities_[%d]= %f\n",i,densities_[i]);
  }
  for(int i=0;i<material_id_of_elements_.size();i++){
    printf("material_id_of_elements_[%d] = %d\n", i, material_id_of_elements_[i]);
  }
}


class IntegrationOverEdge{
public:
  void InitializeIntegrationOverEdge();
  void EdgeIntegration(int, double, std::vector<double>&,std::vector<int>&);
  void PrintDeterminantOfJacobianMatrix();

protected:
  std::vector<std::vector<double> > coordinates_in_this_element_;
  std::vector<double> shape_function_;
  std::vector<std::vector<double> > shape_function_derivatives_;
  double determinant_of_jacobian_matrix_;
};
void IntegrationOverEdge::InitializeIntegrationOverEdge(){
  coordinates_in_this_element_.resize(2);
  coordinates_in_this_element_[0].resize(Constants::kNumOfNodesInElement_, 0.0);
  coordinates_in_this_element_[1].resize(Constants::kNumOfNodesInElement_, 0.0);
  
  shape_function_.resize(Constants::kNumOfNodesInElement_, 0.0);
   
  shape_function_derivatives_.resize(2);
  shape_function_derivatives_[0].resize(Constants::kNumOfNodesInElement_, 0.0);
  shape_function_derivatives_[1].resize(Constants::kNumOfNodesInElement_, 0.0);
}

void IntegrationOverEdge::EdgeIntegration(const int element_number, const double ksi_coordinate, std::vector<double>& x_coordinates, std::vector<int>& nodes_in_elements){
  double Partial_X_Partial_ksi,Partial_Y_Partial_ksi;
  //zero vector and matrices
  int num_of_nodes_in_one_dimensional_element=2;
  for(int k=0;k<num_of_nodes_in_one_dimensional_element;k++){
    coordinates_in_this_element_[0][k]=0.0;
    coordinates_in_this_element_[1][k]=0.0; 
    shape_function_[k]=0.0;
    shape_function_derivatives_[0][k]=0.0;
    shape_function_derivatives_[1][k]=0.0;
  }

 //order: right-->left
  coordinates_in_this_element_[0][0]=x_coordinates[nodes_in_elements[2+element_number*Constants::kNumOfNodesInElement_]];
  coordinates_in_this_element_[0][1]=x_coordinates[nodes_in_elements[3+element_number*Constants::kNumOfNodesInElement_]];

  shape_function_[0] = 0.5*(1+ksi_coordinate);
  shape_function_[1] = 0.5*(1-ksi_coordinate);

  //----------------------to get determinant_of_jacobian_matrix----------------------
  // Calculate the local derivatives of the shape functions.
  shape_function_derivatives_[0][0] = 0.5;
  shape_function_derivatives_[0][1] = -0.5;

  Partial_X_Partial_ksi = shape_function_derivatives_[0][0]*coordinates_in_this_element_[0][0] + 
                          shape_function_derivatives_[0][1]*coordinates_in_this_element_[0][1];

  determinant_of_jacobian_matrix_=fabs(Partial_X_Partial_ksi);
}

void IntegrationOverEdge::PrintDeterminantOfJacobianMatrix(){
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("coordinates_in_this_element_[0][%d] = %f\n", k, coordinates_in_this_element_[0][k]);
    printf("coordinates_in_this_element_[1][%d] = %f\n", k, coordinates_in_this_element_[1][k]);
  }
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("shape_function_[%d] = %f\n", k, shape_function_[k]);
  }
  for(int k=0;k<Constants::kNumOfNodesInElement_;k++){
    printf("shape_function_derivatives_[0][%d] = %f\n", k, shape_function_derivatives_[0][k]);
    printf("shape_function_derivatives_[1][%d] = %f\n", k, shape_function_derivatives_[1][k]);
  }
  printf("determinant_of_jacobian_matrix_ = %f\n", determinant_of_jacobian_matrix_);
}


class ElementalStiffnessMatrix:public MappingShapeFunctionAndDerivatives{
public:
  void InitializeElementalStiffnessMatrix();
  void set_element_stiffness_matrix(int, std::vector<int>&, std::vector<int>&, std::vector<double>&, TemperatureDependentVariables*);
  void MapElementalToGlobalStiffness(std::vector<double>&, std::vector<int>&,std::vector<int>&, int);
  std::vector<std::vector<double> >& get_element_stiffness_matrix(){return element_stiffness_matrix_;}
  void PrintStiffnessMatrix(std::vector<double>&);

private:
  std::vector<std::vector<double> > element_stiffness_matrix_;
};
void ElementalStiffnessMatrix::InitializeElementalStiffnessMatrix(){
  element_stiffness_matrix_.resize(Constants::kNumOfNodesInElement_);
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    element_stiffness_matrix_[i].resize(Constants::kNumOfNodesInElement_,0.0);
  }
  InitializeMappingShapeFunctionAndDerivatives();
}

void ElementalStiffnessMatrix::set_element_stiffness_matrix(const int element_number, std::vector<int>&nodes_in_elements, std::vector<int>& material_id_of_elements, std::vector<double>& current_temperature_field, TemperatureDependentVariables *const temperature_dependent_variables){

  //----------------integration rule------------------
  int num_of_integration_points = 3;
  double coordinates_of_integration_points[3]={-0.7745966692, 0, 0.7745966692}; //gaussian quadrature coordinates
  double weights_of_integration_points[3]={0.5555555555, 0.8888888888, 0.5555555555};//weight of gaussian point

  //---------zero out element K   matrix--------------
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      element_stiffness_matrix_[i][j]=0.0;
    }
  }

  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){ 
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){

      for(int k=0;k<num_of_integration_points;k++){
        double ksi_coordinate = coordinates_of_integration_points[k];  //gaussian piont coordinate
        double ksi_weight = weights_of_integration_points[k];    //weight of gaussian quadrature

        for(int l=0;l<num_of_integration_points;l++){
          double eta_coordinate = coordinates_of_integration_points[l]; 
          double eta_weight = weights_of_integration_points[l];    

          set_shape_function(ksi_coordinate, eta_coordinate);
          set_shape_function_derivatives(ksi_coordinate, eta_coordinate);
          set_determinant_of_jacobian_matrix();  
          set_dn_dx();

          double dni_x=dn_dx_[0][i];
          double dni_y=dn_dx_[1][i];
          double dnj_x=dn_dx_[0][j];
          double dnj_y=dn_dx_[1][j];

          double nx_nx = dni_x*dnj_x*determinant_of_jacobian_matrix_*ksi_weight*eta_weight;
          double ny_ny = dni_y*dnj_y*determinant_of_jacobian_matrix_*ksi_weight*eta_weight;

          double temperature = 0.0;
          for(int ii=0;ii<Constants::kNumOfNodesInElement_;ii++){
            temperature += current_temperature_field[(nodes_in_elements[ii+element_number*4])]*shape_function_[ii];
          }

          element_stiffness_matrix_[i][j] += 
          (*temperature_dependent_variables).get_thermal_conductivity(element_number, temperature, material_id_of_elements)*(nx_nx+ny_ny);
        }
      }
    }
  }
}

void ElementalStiffnessMatrix::MapElementalToGlobalStiffness(std::vector<double>&stiffness_matrix, std::vector<int>&accumulative_half_band_width_vector, std::vector<int>&equation_numbers_in_elements, const int element_number){
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      int row_equation_number=equation_numbers_in_elements[i+element_number*Constants::kNumOfNodesInElement_];
      int column_equation_number=equation_numbers_in_elements[j+element_number*Constants::kNumOfNodesInElement_];
      if(row_equation_number>=0 && column_equation_number>=0 && column_equation_number<=row_equation_number){
        int position_in_desparsed_matrix=accumulative_half_band_width_vector[row_equation_number]-(row_equation_number-column_equation_number);
        stiffness_matrix[position_in_desparsed_matrix] += element_stiffness_matrix_[i][j];
      }
    }  
  }
//printf("map to global stiffness matrix completed\n");
}

void ElementalStiffnessMatrix::PrintStiffnessMatrix(std::vector<double>&stiffness_matrix){
 int size_of_desparsed_stiffness_matrix = stiffness_matrix.size();
 for(int i=0; i<size_of_desparsed_stiffness_matrix; i++)
   printf("stiffness_matrix[%d] = %f\n", i, stiffness_matrix[i]);
}


class ElementalMassMatrix:public MappingShapeFunctionAndDerivatives{
public:
  void InitializeElementalMassMatrix();
  void set_element_mass_matrix(int, std::vector<int>&, std::vector<int>&, std::vector<double>&, TemperatureDependentVariables*, std::vector<double>&, double);
  void MapElementalToGlobalMass(std::vector<double>&, std::vector<int>&, std::vector<int>&, int);
  void PrintMassMatrix(int, std::vector<double>&);

private:
  std::vector<std::vector<double> > element_mass_matrix_;
};
void ElementalMassMatrix::InitializeElementalMassMatrix(){
  element_mass_matrix_.resize(Constants::kNumOfNodesInElement_);
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    element_mass_matrix_[i].resize(Constants::kNumOfNodesInElement_,0.0);
  }
  InitializeMappingShapeFunctionAndDerivatives();
}

void ElementalMassMatrix::set_element_mass_matrix(const int element_number, std::vector<int>&nodes_in_elements, std::vector<int>& material_id_of_elements, std::vector<double>& current_temperature_field, TemperatureDependentVariables *const temperature_dependent_variables, std::vector<double> &densities, const double time_increment){
  //----------------integration rule------------------
  int num_of_integration_points = 3;
  double coordinates_of_integration_points[3]={-0.7745966692, 0, 0.7745966692}; //gaussian quadrature coordinates
  double weights_of_integration_points[3]={0.5555555555, 0.8888888888, 0.5555555555};//weight of gaussian point

  //---------zero out element K   matrix--------------
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      element_mass_matrix_[i][j]=0.0;
    }
  }

  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){ 
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      for(int k=0;k<num_of_integration_points;k++){
        double ksi_coordinate = coordinates_of_integration_points[k];  //gaussian piont coordinate
        double ksi_weight = weights_of_integration_points[k];    //weight of gaussian quadrature

        for(int l=0;l<num_of_integration_points;l++){
          double eta_coordinate = coordinates_of_integration_points[l]; 
          double eta_weight = weights_of_integration_points[l];    

          set_shape_function(ksi_coordinate, eta_coordinate);
          set_shape_function_derivatives(ksi_coordinate, eta_coordinate);
          set_determinant_of_jacobian_matrix();  

          double temperature = 0.0;
          for(int ii=0;ii<Constants::kNumOfNodesInElement_;ii++){
            temperature += current_temperature_field[(nodes_in_elements[ii+element_number*4])]*shape_function_[ii];
          }

          double density = densities[material_id_of_elements[element_number]];

          element_mass_matrix_[i][j] += 
          (*temperature_dependent_variables).get_specific_heat(element_number, temperature, material_id_of_elements)*density
          /time_increment*shape_function_[i]*shape_function_[j]*determinant_of_jacobian_matrix_*ksi_weight*eta_weight;;
        }
      }
    }
  }
}

void ElementalMassMatrix::MapElementalToGlobalMass(std::vector<double>&mass_matrix, std::vector<int>&accumulative_half_band_width_vector, std::vector<int>&equation_numbers_in_elements, const int element_number){
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      int row_equation_number=equation_numbers_in_elements[i+element_number*Constants::kNumOfNodesInElement_];
      int column_equation_number=equation_numbers_in_elements[j+element_number*Constants::kNumOfNodesInElement_];
      if(row_equation_number>=0 && column_equation_number>=0 && column_equation_number<=row_equation_number){
        int position_in_desparsed_matrix=accumulative_half_band_width_vector[row_equation_number]-(row_equation_number-column_equation_number);
        mass_matrix[position_in_desparsed_matrix] += element_mass_matrix_[i][j];
      }
    }  
  }
//printf("map to global stiffness matrix completed\n");
}

void ElementalMassMatrix::PrintMassMatrix(int size_of_desparsed_stiffness_matrix, std::vector<double>& mass_matrix){
 for(int i=0; i<size_of_desparsed_stiffness_matrix; i++)
   printf("mass_matrix[%d] = %f\n", i, mass_matrix[i]);
}


class ElementalBodyHeatFluxTangentialMatrix:public MappingShapeFunctionAndDerivatives{
public:
  void InitializeElementalBodyHeatFluxTangentialMatrix();
  void set_element_body_heat_flux_tangential_matrix(int, int, std::vector<int>&, std::vector<double>&, TemperatureDependentVariables*const, 
    Initialization*const);
  void MapElementalToGlobalBodyHeatFluxTangentialMatrix(std::vector<double>&, std::vector<int>&,std::vector<int>&, int);
  void PrintBodyHeatFluxTangentialMatrix(std::vector<double>&);

private:
  std::vector<std::vector<double> > element_body_heat_flux_tangential_matrix_;
};
void ElementalBodyHeatFluxTangentialMatrix::InitializeElementalBodyHeatFluxTangentialMatrix(){
  element_body_heat_flux_tangential_matrix_.resize(Constants::kNumOfNodesInElement_);
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    element_body_heat_flux_tangential_matrix_[i].resize(Constants::kNumOfNodesInElement_,0.0);
  }
  InitializeMappingShapeFunctionAndDerivatives();
}

void ElementalBodyHeatFluxTangentialMatrix::set_element_body_heat_flux_tangential_matrix(const int element_number, const int heater_element_number, std::vector<int>&nodes_in_elements, std::vector<double>& current_temperature_field, TemperatureDependentVariables *const temperature_dependent_variables,Initialization *const initialization){
  int heater_number=heater_element_number/(*((*initialization).get_mesh_parameters())).get_mesh_seeds_on_heater();
  double current=(*((*initialization).get_currents_in_heater())).get_current_in_heater()[heater_number];

  //integration rule
  int num_of_integration_points=2;
  double coordinates_of_integration_points[2]={-0.57735026, 0.57735026}; //gaussian quadrature coordinates
  double weights_of_integration_points[2]={1.0, 1.0};//weight of gaussian point

  //---------zero out element tangential body heat flux matrix--------------
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++)
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++)
      element_body_heat_flux_tangential_matrix_[i][j]=0.0;
  
  //---------------------------------
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){ 
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){

      for(int k=0;k<num_of_integration_points;k++){
        double ksi_coordinate=coordinates_of_integration_points[k];  //gaussian piont coordinate
        double ksi_weight=weights_of_integration_points[k];    //weight of gaussian quadrature
        for(int l=0;l<num_of_integration_points;l++){
          double eta_coordinate=coordinates_of_integration_points[l]; 
          double eta_weight=weights_of_integration_points[l];  

          set_shape_function(ksi_coordinate, eta_coordinate);
          set_shape_function_derivatives(ksi_coordinate, eta_coordinate);
          set_determinant_of_jacobian_matrix();  
      
          double temperature=0.0;
          for(int ii=0;ii<Constants::kNumOfNodesInElement_;ii++)
            temperature += current_temperature_field[(nodes_in_elements[ii+element_number*4])]*shape_function_[ii];

          element_body_heat_flux_tangential_matrix_[i][j] += (*temperature_dependent_variables).get_body_heat_flux_derivative
          (temperature,current)*shape_function_[i]*shape_function_[j]*determinant_of_jacobian_matrix_*ksi_weight*eta_weight;
        }
      }
    }  
  }
}

void ElementalBodyHeatFluxTangentialMatrix::MapElementalToGlobalBodyHeatFluxTangentialMatrix(std::vector<double>&body_heat_flux_tangential_matrix,std::vector<int>&accumulative_half_band_width_vector,std::vector<int>&equation_numbers_in_elements, const int element_number){
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      int row_equation_number=equation_numbers_in_elements[i+element_number*Constants::kNumOfNodesInElement_];
      int column_equation_number=equation_numbers_in_elements[j+element_number*Constants::kNumOfNodesInElement_];
      if(row_equation_number>=0 && column_equation_number>=0 && column_equation_number<=row_equation_number){
        int position_in_desparsed_matrix=accumulative_half_band_width_vector[row_equation_number]-(row_equation_number-column_equation_number);
        body_heat_flux_tangential_matrix[position_in_desparsed_matrix] += element_body_heat_flux_tangential_matrix_[i][j];
      }
    }  
  }
//printf("map to global stiffness matrix completed\n");
}

void ElementalBodyHeatFluxTangentialMatrix::PrintBodyHeatFluxTangentialMatrix(std::vector<double>& body_heat_flux_tangential_matrix){
 int size_of_desparsed_stiffness_matrix = body_heat_flux_tangential_matrix.size();
 for(int i=0; i<size_of_desparsed_stiffness_matrix; i++)
   printf("body_heat_flux_tangential_matrix[%d] = %f\n", i, body_heat_flux_tangential_matrix[i]);
}


class ElementalRadiationTangentialMatrixAndRadiationLoad: public IntegrationOverEdge{
public:
  void InitializeElementalRadiationTangentialMatrixAndRadiationLoad();
  void set_element_radiation_tangential_matrix_and_radiation_load(int, int, std::vector<int>&, std::vector<double>&, 
  TemperatureDependentVariables*, std::vector<double>&, double);
  void MapElementalToGlobalRadiationTangentialMatrixAndRadiationLoad(std::vector<double>&, std::vector<int>&, std::vector<double>&, 
  std::vector<int>&, int);
  void PrintRadiationTangentialMatrixAndRadiationLoad(std::vector<double>&, std::vector<double>&);

private:
  std::vector<std::vector<double> > element_radiation_tangential_matrix_;
  double local_radiation_load[2];
};
void ElementalRadiationTangentialMatrixAndRadiationLoad::InitializeElementalRadiationTangentialMatrixAndRadiationLoad(){
  element_radiation_tangential_matrix_.resize(Constants::kNumOfNodesInElement_);
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    element_radiation_tangential_matrix_[i].resize(Constants::kNumOfNodesInElement_,0.0);
  }
  InitializeIntegrationOverEdge();
}

void ElementalRadiationTangentialMatrixAndRadiationLoad::set_element_radiation_tangential_matrix_and_radiation_load(const int element_number, const int radiation_element_number, std::vector<int>&nodes_in_elements, std::vector<double>& current_temperature_field, TemperatureDependentVariables *const temperature_dependent_variables,std::vector<double>&x_coordinates, const double ambient_temperature){
  //integration rule 
  int num_of_integration_points=4;
  double coordinates_of_integration_points[4]={-0.861136312, -0.339981044, 0.339981044, 0.861136312}; //gaussian quadrature coordinates
  double weights_of_integration_points[4]={0.347854845, 0.652145155, 0.652145155, 0.347854845};//weight of gaussian point

  //---------zero out element Tangentialradiation matrix--------------
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      element_radiation_tangential_matrix_[j][i]=0.0;
    }
  }

  for(int i=0;i<2;i++)
    local_radiation_load[i]=0.0;

  double temperature_quartic_o=pow(ambient_temperature,4);

  for(int k=0;k<num_of_integration_points;k++){
    double ksi_coordinate=coordinates_of_integration_points[k];  //gaussian piont coordinate
    double ksi_weight=weights_of_integration_points[k];    //weight of gaussian quadrature
    
    EdgeIntegration(element_number,ksi_coordinate, x_coordinates, nodes_in_elements);

    double temperature=0.0;
    temperature += current_temperature_field[(nodes_in_elements[2+element_number*4])]*shape_function_[0];
    temperature += current_temperature_field[(nodes_in_elements[3+element_number*4])]*shape_function_[1];

    double temperature_cube=pow(temperature,3);
    double temperature_quartic=pow(temperature,4);
    
    double constant_a=Constants::kStefanBoltzmann_*(*temperature_dependent_variables).get_emissivity(temperature);
    double constant_a_derivative=Constants::kStefanBoltzmann_*(*temperature_dependent_variables).get_emissivity_derivative(temperature);

    double coefficient=4*constant_a*temperature_cube+constant_a_derivative*temperature_quartic;

    //note that body heat flux and radiation heat flux are of OPPSITE sign! one increases temperature, while the other one drecreases it.
    element_radiation_tangential_matrix_[2][2]+=coefficient*shape_function_[0]*shape_function_[0]*determinant_of_jacobian_matrix_*ksi_weight;
    element_radiation_tangential_matrix_[2][3]+=coefficient*shape_function_[0]*shape_function_[1]*determinant_of_jacobian_matrix_*ksi_weight;
    element_radiation_tangential_matrix_[3][2]+=coefficient*shape_function_[1]*shape_function_[0]*determinant_of_jacobian_matrix_*ksi_weight;
    element_radiation_tangential_matrix_[3][3]+=coefficient*shape_function_[1]*shape_function_[1]*determinant_of_jacobian_matrix_*ksi_weight;

    //calculate radiation load
    local_radiation_load[0] += shape_function_[0]*constant_a*(temperature_quartic-temperature_quartic_o)
    *determinant_of_jacobian_matrix_*ksi_weight;   //3rd node in this element
    local_radiation_load[1] += shape_function_[1]*constant_a*(temperature_quartic-temperature_quartic_o)
    *determinant_of_jacobian_matrix_*ksi_weight;   //4th node in this element
  }
}


void ElementalRadiationTangentialMatrixAndRadiationLoad::MapElementalToGlobalRadiationTangentialMatrixAndRadiationLoad(std::vector<double>&radiation_tangential_matrix, std::vector<int>&accumulative_half_band_width_vector, std::vector<double>&radiation_load,std::vector<int>&equation_numbers_in_elements, const int element_number){
  for(int i=0;i<Constants::kNumOfNodesInElement_;i++){
    for(int j=0;j<Constants::kNumOfNodesInElement_;j++){
      int row_equation_number=equation_numbers_in_elements[i+element_number*Constants::kNumOfNodesInElement_];
      int column_equation_number=equation_numbers_in_elements[j+element_number*Constants::kNumOfNodesInElement_];
      if(row_equation_number>=0 && column_equation_number>=0 && column_equation_number<=row_equation_number){
        int position_in_desparsed_matrix=accumulative_half_band_width_vector[row_equation_number]-(row_equation_number-column_equation_number);
        radiation_tangential_matrix[position_in_desparsed_matrix] += element_radiation_tangential_matrix_[i][j];
      }
    }  
  }
  //map to radiation load
  radiation_load[equation_numbers_in_elements[element_number*Constants::kNumOfNodesInElement_+2]] += local_radiation_load[0];
  radiation_load[equation_numbers_in_elements[element_number*Constants::kNumOfNodesInElement_+3]] += local_radiation_load[1];
//printf("map to global Radiation Tangential Matrix And Radiatio nLoad completed\n");
}

void ElementalRadiationTangentialMatrixAndRadiationLoad::PrintRadiationTangentialMatrixAndRadiationLoad
(std::vector<double>& radiation_tangential_matrix, std::vector<double>& radiation_load){
 int size_of_desparsed_stiffness_matrix = radiation_tangential_matrix.size();
 for(int i=0; i<size_of_desparsed_stiffness_matrix; i++)
   printf("radiation_tangential_matrix[%d] = %f\n", i, radiation_tangential_matrix[i]);
 int size_of_load = radiation_load.size();
 for(int i=0; i<size_of_load; i++)
   printf("radiation_load[%d] = %f\n", i, radiation_load[i]);
}


class GlobalVectorsAndMatrices{
public:
  void InitializeGlobalVectorsAndMatrices(int, std::vector<int>&);
  std::vector<double>& get_stiffness_matrix()
    {return stiffness_matrix_;}
  std::vector<double>& get_mass_matrix()
    {return mass_matrix_;}
  std::vector<double>& get_radiation_tangential_matrix()
    {return radiation_tangential_matrix_;}
  std::vector<double>& get_body_heat_flux_tangential()
    {return body_heat_flux_tangential_matrix_;}
  std::vector<double>& get_heat_load()
    {return heat_load_;}
  std::vector<double>& get_radiation_load()
    {return radiation_load_;}
  std::vector<double>& get_current_temperature_field()
    {return current_temperature_field_;}
  std::vector<double>& get_right_hand_side_function()
    {return right_hand_side_function_;}
  std::vector<double>& get_jacobian_matrix_global()
    {return jacobian_matrix_global_;}
  std::vector<double>& get_solution_increments_trial()
    {return solution_increments_trial_;}
  std::vector<double>& get_initial_temperature_field()
    {return initial_temperature_field_;}
  std::vector<double>& get_solution_of_last_iteration()
    {return solution_of_last_iteration_;}
  void PrintGlobalVectorsAndMatrices(){
    for(int i=0;i<stiffness_matrix_.size();i++)
      printf("stffness_matrix_[%d] = %f\n", i, stiffness_matrix_[i]);
  }
  double UseIndexToSearchDoubleSymmetricMatrix(std::vector<double>*, int, int);
  double UseIndexToSearchDoubleAsymmetricMatrix(std::vector<double>*, std::vector<double>*, int, int);
  double StiffnessMatrixIndex(int, int);
  double MassMatrixIndex(int, int);
  double RadiationTangentialMatrixIndex(int, int);
  double BodyHeatFluxTangentialMatrixIndex(int, int);
  double JacobianMatrixIndex(int, int);
  int ModifyJacobianMatrixIndex(int, int, double);
  void ZeroVectorAndMatrix();

private:
  std::vector<double> stiffness_matrix_;
  std::vector<double> mass_matrix_;
  std::vector<double> radiation_tangential_matrix_;
  std::vector<double> body_heat_flux_tangential_matrix_;
  std::vector<double> heat_load_;
  std::vector<double> radiation_load_;
  std::vector<double> current_temperature_field_;
  std::vector<double> right_hand_side_function_; //dGlobalyfunc
  std::vector<double> jacobian_matrix_global_;
  std::vector<double> solution_increments_trial_;//dIterstep
  std::vector<double> initial_temperature_field_;
  std::vector<double> solution_of_last_iteration_;
  std::vector<int> accumulative_half_band_width_vector_;
  int num_of_nodes_;
  int num_of_equations_;
};
void GlobalVectorsAndMatrices::InitializeGlobalVectorsAndMatrices(const int num_of_nodes, std::vector<int>&accumulative_half_band_width_vector){
  int num_of_equations = accumulative_half_band_width_vector.size();
  int size_of_desparsed_stiffness_matrix = accumulative_half_band_width_vector[num_of_equations-1]+1;
  stiffness_matrix_.resize(size_of_desparsed_stiffness_matrix, 0.0);
  mass_matrix_.resize(size_of_desparsed_stiffness_matrix, 0.0);
  radiation_tangential_matrix_.resize(size_of_desparsed_stiffness_matrix, 0.0);
  body_heat_flux_tangential_matrix_.resize(size_of_desparsed_stiffness_matrix, 0.0);
  jacobian_matrix_global_.resize(size_of_desparsed_stiffness_matrix, 0.0);
  heat_load_.resize(num_of_equations, 0.0);
  radiation_load_.resize(num_of_equations, 0.0);
  right_hand_side_function_.resize(num_of_equations, 0.0);
  solution_increments_trial_.resize(num_of_equations, 0.0);
  solution_of_last_iteration_.resize(num_of_equations, 0.0);
  initial_temperature_field_.resize(num_of_nodes, 0.0);
  current_temperature_field_.resize(num_of_nodes, 0.0);
  accumulative_half_band_width_vector_ = accumulative_half_band_width_vector;
  num_of_nodes_=num_of_nodes;
  num_of_equations_=num_of_equations;
}

double GlobalVectorsAndMatrices::UseIndexToSearchDoubleSymmetricMatrix(std::vector<double>* desparsed_matrix, int i, int j){
  int first_column;
  double return_value;
  if(j<num_of_equations_){
    if(j>i){int temporary_variable=i; i=j; j=temporary_variable;}

    if(i==0)  first_column=0;
    else first_column=i-(accumulative_half_band_width_vector_[i]-accumulative_half_band_width_vector_[i-1])+1;  //the column number of the 1st nonezero component in i row (all 0 indexed)

    if(j<first_column) return_value = 0.0;
    else{
      int ij=accumulative_half_band_width_vector_[i]-(i-j);
      return_value = (*desparsed_matrix)[ij];
    }
  }
  else if(j==num_of_equations_) return_value = right_hand_side_function_[i];
  return return_value;
}

int GlobalVectorsAndMatrices::ModifyJacobianMatrixIndex(int i, int j, double new_value){
  int first_column;
  if(j>i && j!=num_of_equations_){//upper triangular part  j must greater than or equal to 1
    double ij=j;
    j=i;
    i=ij;   
  }

  if(j<=i){//lower triangular part
    if(i==0)  first_column=0;
    else  first_column=i-(accumulative_half_band_width_vector_[i]-accumulative_half_band_width_vector_[i-1])+1;  //the column number of the 1st nonezero component in i row (all 0 indexed)
    if(j<first_column && new_value!=0.0) return 1;
    if(j>=first_column){
      int ij=accumulative_half_band_width_vector_[i]-(i-j);
      jacobian_matrix_global_[ij]=new_value;
    }
  }

  else if(j==num_of_equations_){
    right_hand_side_function_[i]=new_value;
  }
  return 0;
}

double GlobalVectorsAndMatrices::StiffnessMatrixIndex(const int i, const int j){
  return UseIndexToSearchDoubleSymmetricMatrix(&stiffness_matrix_, i, j);
}

double GlobalVectorsAndMatrices::MassMatrixIndex(const int i, const int j){
  return UseIndexToSearchDoubleSymmetricMatrix(&mass_matrix_, i, j);
}

double GlobalVectorsAndMatrices::RadiationTangentialMatrixIndex(const int i, const int j){
  return UseIndexToSearchDoubleSymmetricMatrix(&radiation_tangential_matrix_, i, j);
}

double GlobalVectorsAndMatrices::BodyHeatFluxTangentialMatrixIndex(const int i, const int j){
  return UseIndexToSearchDoubleSymmetricMatrix(&body_heat_flux_tangential_matrix_, i, j);
}

double GlobalVectorsAndMatrices::JacobianMatrixIndex(const int i, const int j){
  return UseIndexToSearchDoubleSymmetricMatrix(&jacobian_matrix_global_, i, j);
}

void GlobalVectorsAndMatrices::ZeroVectorAndMatrix(){
  for(int i=0; i<stiffness_matrix_.size(); i++){
    stiffness_matrix_[i]=0.0;
    mass_matrix_[i]=0.0;
    radiation_tangential_matrix_[i]=0.0;
    body_heat_flux_tangential_matrix_[i]=0.0;
    jacobian_matrix_global_[i]=0.0;
  }
  for(int i=0; i<heat_load_.size(); i++){
    heat_load_[i]=0.0;
    radiation_load_[i]=0.0;
    right_hand_side_function_[i]=0.0;
  }
}



class Assemble{
public:
  void AssembleGlobalJacobian(GlobalVectorsAndMatrices*);
  void AssembleGlobalYfunction(std::vector<int>&, GlobalVectorsAndMatrices*);
  void PrintGlobalJacobian(GlobalVectorsAndMatrices*);
  void PrintGlobalYfunction(GlobalVectorsAndMatrices*);
};
void Assemble::AssembleGlobalJacobian(GlobalVectorsAndMatrices* global_vectors_and_matrices){
  double new_value;
  int size_of_desparsed_matrix = ((*global_vectors_and_matrices).get_jacobian_matrix_global()).size();
  for(int i=0; i<size_of_desparsed_matrix; i++){
    (*global_vectors_and_matrices).get_jacobian_matrix_global()[i] = (*global_vectors_and_matrices).get_mass_matrix()[i] 
                                                                 +(*global_vectors_and_matrices).get_stiffness_matrix()[i] 
                                                                 +(*global_vectors_and_matrices).get_radiation_tangential_matrix()[i] 
                                                                 +(*global_vectors_and_matrices).get_body_heat_flux_tangential()[i];
  }
}

void Assemble::AssembleGlobalYfunction(std::vector<int>& equation_numbers_of_nodes, GlobalVectorsAndMatrices* global_vectors_and_matrices){
  int num_of_nodes=equation_numbers_of_nodes.size();
  int row_equation_number=-1;
  int column_equation_number=-1;
  double term_one=0.0;
  double term_two=0.0;
  for(int i=0;i<num_of_nodes;i++){
    term_one=0.0;
    term_two=0.0;
    for(int j=0;j<num_of_nodes;j++){
      row_equation_number=equation_numbers_of_nodes[i];
      column_equation_number=equation_numbers_of_nodes[j];
      if(row_equation_number>=0 && column_equation_number>=0){
        term_one += (*global_vectors_and_matrices).MassMatrixIndex(row_equation_number,column_equation_number)*
                    ((*global_vectors_and_matrices).get_current_temperature_field()[j]
                    -(*global_vectors_and_matrices).get_initial_temperature_field()[j]);
        term_two += ((*global_vectors_and_matrices).StiffnessMatrixIndex(row_equation_number,column_equation_number))*
                    ((*global_vectors_and_matrices).get_current_temperature_field()[j]);
      }
    }

    if(row_equation_number>=0 && column_equation_number>=0){
    (*global_vectors_and_matrices).get_right_hand_side_function()[row_equation_number]= 
       ((*global_vectors_and_matrices).get_heat_load()[row_equation_number]) - 
       ((*global_vectors_and_matrices).get_radiation_load()[row_equation_number])-term_one-term_two; 
    }
  }
//printf("Generating R.H.S global function completed\n");
}

void Assemble::PrintGlobalJacobian(GlobalVectorsAndMatrices* global_vectors_and_matrices){
  int num_of_equations = ((*global_vectors_and_matrices).get_right_hand_side_function()).size();
  for(int i=0; i<num_of_equations; i++)
    for(int j=0; j<num_of_equations; j++)
      printf("JacobianMatrixIndex(%d,%d) = %f\n", i, j, (*global_vectors_and_matrices).JacobianMatrixIndex(i,j));
}

void Assemble::PrintGlobalYfunction(GlobalVectorsAndMatrices* global_vectors_and_matrices){
  int num_of_equations = ((*global_vectors_and_matrices).get_right_hand_side_function()).size();
  for(int i=0; i<num_of_equations; i++)
    printf("Heat load(%d) = %-12.6f   Radiation load(%d) = %-12.6f   GlobalYfunction(%d) = %-12.6f\n", i, (*global_vectors_and_matrices).get_heat_load()[i], i, (*global_vectors_and_matrices).get_radiation_load()[i], i, (*global_vectors_and_matrices).get_right_hand_side_function()[i]);
}


class Solver{
public:
  double NormOfVector(std::vector<double>&);
  int LinearEquationsSolver(GlobalVectorsAndMatrices *);
};
double Solver::NormOfVector(std::vector<double>& vector_to_be_evaluated){
  double return_value=0.0;
  double summation_over_squred_components=0.0;
  for(int i=0; i<vector_to_be_evaluated.size(); i++)
    summation_over_squred_components += (vector_to_be_evaluated[i])*(vector_to_be_evaluated[i]);
  return_value=(pow(summation_over_squred_components,0.5));
return return_value;
}

int Solver::LinearEquationsSolver(GlobalVectorsAndMatrices *global_vectors_and_matrices){
//use LinearEquationsSolver to solve equations
  int num_of_equations = ((*global_vectors_and_matrices).get_right_hand_side_function()).size();
  double temporary_variable, new_value;
//   decomposition 
  for(int k=0; k<num_of_equations; k++){   
    temporary_variable = 0.0;   
    for(int i=0; i<k; i++){   
      temporary_variable += (((*global_vectors_and_matrices).JacobianMatrixIndex(i,k))*
                            ((*global_vectors_and_matrices).JacobianMatrixIndex(i,k)));  
    }
    new_value=(double)sqrt( ((*global_vectors_and_matrices).JacobianMatrixIndex(k,k)) - temporary_variable);
    if(((*global_vectors_and_matrices).ModifyJacobianMatrixIndex(k,k,new_value))) return 1;
    for(int i=k+1; i<num_of_equations+1; i++){   
      temporary_variable = 0.0;   
      for(int j=0; j<k; j++){   
        temporary_variable += ((*global_vectors_and_matrices).JacobianMatrixIndex(j,k))
                             *((*global_vectors_and_matrices).JacobianMatrixIndex(j,i));   
      }   
      new_value= (((*global_vectors_and_matrices).JacobianMatrixIndex(k,i))-temporary_variable)
                 /((*global_vectors_and_matrices).JacobianMatrixIndex(k,k));
      if(((*global_vectors_and_matrices).ModifyJacobianMatrixIndex(k,i,new_value))) return 1;   
    }   
  }   

//  back substitution   
  new_value= ((*global_vectors_and_matrices).JacobianMatrixIndex(num_of_equations-1,num_of_equations)) 
             /((*global_vectors_and_matrices).JacobianMatrixIndex(num_of_equations-1,num_of_equations-1));
  if(((*global_vectors_and_matrices).ModifyJacobianMatrixIndex(num_of_equations-1,num_of_equations,new_value))) return 1;
  
  for(int k=num_of_equations-2; k>=0; k--){   
    temporary_variable = 0;   
    for(int i=k+1; i<num_of_equations; i++){   
      temporary_variable +=  ((*global_vectors_and_matrices).JacobianMatrixIndex(k,i))
                           *((*global_vectors_and_matrices).JacobianMatrixIndex(i,num_of_equations));   
    }   

    new_value=(((*global_vectors_and_matrices).JacobianMatrixIndex(k,num_of_equations)) - temporary_variable) 
              /((*global_vectors_and_matrices).JacobianMatrixIndex(k,k)); 
    if(((*global_vectors_and_matrices).ModifyJacobianMatrixIndex(k,num_of_equations,new_value))) return 1;   
  }   

//  store disp into solution_of_last_iteration vector/
  for(int i=0; i<num_of_equations; i++){   
    (*global_vectors_and_matrices).get_solution_of_last_iteration()[i]=((*global_vectors_and_matrices).JacobianMatrixIndex(i,num_of_equations));   
  }
  
//  printf("Solving linear equations completed......\n");
  return 0;
}  


class OutputResults{
public:
  void OutputVtkFile(int, double, Initialization*, GenerateMesh*, std::vector<double>&);
  void OutputCopperSurfaceTemperature(int, double, Initialization*, GenerateMesh*, std::vector<double>&);
};
void OutputResults::OutputVtkFile(const int time_step, const double current_time, Initialization *const initialization, GenerateMesh *const generate_mesh, std::vector<double>& initial_temperature_field){   
  int num_of_nodes=(*((*initialization).get_mesh_parameters())).get_num_of_nodes();
  int dimensions_of_x=(*((*initialization).get_mesh_parameters())).get_dimensions_of_x();
  int dimensions_of_y=(*((*initialization).get_mesh_parameters())).get_dimensions_of_y();

  char buffer[20];
  sprintf(buffer, "step_%d.vtk", time_step+1);
  char *filename = buffer; 
  FILE *output_vtk_file;
  output_vtk_file = fopen(filename,"w"); //analysis results stored in Output.txt
  if(output_vtk_file==NULL){
    printf("cann't open the file !\n");
    exit(1);
  }
//  printf("open Output file succeeded\n");
  fprintf(output_vtk_file,"# vtk DataFile Version 2.0\n");
  fprintf(output_vtk_file,"current time is %f\n", current_time);
  fprintf(output_vtk_file,"ASCII\n");
  fprintf(output_vtk_file,"DATASET STRUCTURED_GRID\n"); 
  fprintf(output_vtk_file,"DIMENSIONS %d %d 1\n", dimensions_of_x,dimensions_of_y); 
  fprintf(output_vtk_file,"POINTS %d double\n", num_of_nodes); 
  for(int j=0;j<num_of_nodes;j++){
    fprintf(output_vtk_file,"%.8f  %.8f  0.0\n",(*generate_mesh).get_x_coordinates()[j], (*generate_mesh).get_y_coordinates()[j]);
  }
  fprintf(output_vtk_file, "POINT_DATA %d\n", num_of_nodes);
  fprintf(output_vtk_file, "SCALARS temperature double\n");
  fprintf(output_vtk_file, "LOOKUP_TABLE default\n");
  for(int j=0;j<num_of_nodes;j++){
    fprintf(output_vtk_file,"%.8f\n",initial_temperature_field[j]);
  }
  fclose(output_vtk_file);
  printf("writing to vtk file completed......\n");
}

void OutputResults::OutputCopperSurfaceTemperature(const int time_step, const double current_time, Initialization *const initialization, GenerateMesh *const generate_mesh, std::vector<double>& initial_temperature_field){
  std::vector<std::pair<double,double> > nodes_on_copper_surface;
  int num_of_nodes=(*((*initialization).get_mesh_parameters())).get_num_of_nodes();
  double y_coordinate_of_copper_surface = (*((*initialization).get_model_geometry())).get_thickness_of_csilicon()
                                         +(*((*initialization).get_model_geometry())).get_thickness_of_isolater()
                                         +(*((*initialization).get_model_geometry())).get_thickness_of_titanium()
                                         +(*((*initialization).get_model_geometry())).get_thickness_of_silicondioxide() 
                                         +(*((*initialization).get_model_geometry())).get_thickness_of_copper();
  double x_left_bound=(*((*initialization).get_model_geometry())).get_width_of_end(); 
  double x_right_bound=(*((*initialization).get_model_geometry())).get_length_of_model()-(*((*initialization).get_model_geometry())).get_width_of_end();
  double tolerance=1.0e-5;

  char buffer[20];
  sprintf(buffer, "plotdata_step_%d.txt", time_step+1);
  char *filename = buffer; 
  FILE *output_copper_surface_temperature;
  output_copper_surface_temperature=fopen(filename,"w"); 
  if(output_copper_surface_temperature==NULL){
    printf("cann't open the file !\n");
    exit(1);
  }

  for(int j=0;j<num_of_nodes;j++){
    if( fabs((*generate_mesh).get_y_coordinates()[j]-y_coordinate_of_copper_surface)<tolerance 
      && (*generate_mesh).get_x_coordinates()[j]>x_left_bound-tolerance 
      && (*generate_mesh).get_x_coordinates()[j]<x_right_bound+tolerance ){
      double x_coordinate=(*generate_mesh).get_x_coordinates()[j]
                          -(*((*initialization).get_model_geometry())).get_width_of_end();
      double temperature=initial_temperature_field[j];
      nodes_on_copper_surface.push_back(std::make_pair(x_coordinate,temperature));
    }
  } 

  fprintf(output_copper_surface_temperature,"current time is %f\n", current_time);
  for(int i=0;i<(nodes_on_copper_surface.size()-1);i++){
    for(int j=i+1;j<nodes_on_copper_surface.size();j++){
      if(nodes_on_copper_surface[i].first>nodes_on_copper_surface[j].first){
        double temporary_variable=nodes_on_copper_surface[i].first;
        nodes_on_copper_surface[i].first=nodes_on_copper_surface[j].first;
        nodes_on_copper_surface[j].first=temporary_variable;
        temporary_variable=nodes_on_copper_surface[i].second; 
        nodes_on_copper_surface[i].second=nodes_on_copper_surface[j].second;
        nodes_on_copper_surface[j].second=temporary_variable;
      }
    }
    fprintf(output_copper_surface_temperature,"%.8f\t%.10f\t\n",nodes_on_copper_surface[i].first,nodes_on_copper_surface[i].second);
  }
  fprintf(output_copper_surface_temperature,"%.8f\t%.10f\t\n",nodes_on_copper_surface[(nodes_on_copper_surface.size()-1)].first,
  nodes_on_copper_surface[(nodes_on_copper_surface.size()-1)].second);

  fclose(output_copper_surface_temperature);
  std::cout<<"writing copper surface temperature completed\n"<<std::endl;
}


int main(){
  printf("\n\n\t*****Heat Transfer Simulation for Real Time Grain Growth Control of Copper Film*****\n");
  printf("\tThis code is developed for the project 'Real Time Control of Grain Growth in Metals' (NSF reference codes: 024E, 036E, 8022, AMPP)\n\n");
  printf("\tProject Investigators:\n");
  printf("\tRobert Hull hullr2@rpi.edu (Principal Investigator)\n\tJohn Wen (Co-Principal Investigator)\n\tAntoinette Maniatty (Co-Principal Investigator)\n\tDaniel Lewis (Co-Principal Investigator)\n\n");
  printf("\tCode developer: Yixuan Tan tany3@rpi.edu\n\n\n");
 
  Initialization initialization;
  initialization.InitializeInitialization();
  double ambient_temperature=(*(initialization.get_analysis_constants())).get_ambient_temperature();
  double initial_time_increment=(*(initialization.get_analysis_constants())).get_initial_time_increment();
  double minimum_time_increment=(*(initialization.get_analysis_constants())).get_minimum_time_increment();
  int maximum_time_steps=(*(initialization.get_analysis_constants())).get_maximum_time_steps();
  double total_simulation_time=(*(initialization.get_analysis_constants())).get_total_simulation_time();
  double maximum_temperature_change_per_time_increment=(*(initialization.get_analysis_constants())).get_maximum_temperature_change_per_time_increment();
  double time_to_turn_off_heaters=(*(initialization.get_analysis_constants())).get_time_to_turn_off_heaters();
  int output_time_step_interval=(*(initialization.get_analysis_constants())).get_output_time_step_interval();
  int num_of_nodes = (*(initialization.get_mesh_parameters())).get_num_of_nodes();
  int num_of_elements = (*(initialization.get_mesh_parameters())).get_num_of_elements();
  double time_increment=initial_time_increment;
  int num_of_iterations_with_unchanged_time_increment=0;
  int iteration_number=0;
  double current_time=0.0;
  bool check_temperature_change_size_satisfiable;
  bool is_heaters_turned_off=false;
  double temperature_norm_last = 0.0;
  double temperature_norm_current = 0.0;


  //read input data to generate mesh
  GenerateMesh generate_mesh;
  generate_mesh.GenerateMeshInitializeMeshSizeInfo(&initialization);
  generate_mesh.CalculateCoordinates(&initialization);
//  generate_mesh.PrintCoordinatesResults(&initialization);
  std::vector<double> &x_coordinates = generate_mesh.get_x_coordinates();
  std::vector<double> &y_coordinates = generate_mesh.get_y_coordinates();

  DegreeOfFreedomAndEquationNumbers dof_and_equation_numbers;
  dof_and_equation_numbers.InitializeDegreeOfFreedomAndEquationNumbers(&initialization);
  dof_and_equation_numbers.set_essential_bc_nodes(&initialization, y_coordinates);
  dof_and_equation_numbers.GenerateNodeAndEquationNumbersInElements(&initialization);
//  dof_and_equation_numbers.PrintDofAndEquationNumbers(&initialization);
  std::vector<int> &nodes_in_elements = dof_and_equation_numbers.get_nodes_in_elements();
  int num_of_essential_bc_nodes = dof_and_equation_numbers.get_num_of_essential_bc_nodes();
  std::vector<int> &essential_bc_nodes = dof_and_equation_numbers.get_essential_bc_nodes();
  int num_of_equations = dof_and_equation_numbers.get_num_of_equations();
  std::vector<int> &equation_numbers_of_nodes = dof_and_equation_numbers.get_equation_numbers_of_nodes();
  std::vector<int> &equation_numbers_in_elements = dof_and_equation_numbers.get_equation_numbers_in_elements();

  TemperatureDependentVariables temperature_dependent_variables;
  temperature_dependent_variables.InitializeTemperatureDependentVariables(&initialization);

  HalfBandWidth half_band_width;
  half_band_width.set_accumulative_half_band_width_vector(&initialization, &dof_and_equation_numbers);
//  half_band_width.PrintHalfBandWidthInformation(num_of_equations);
  int size_of_desparsed_stiffness_matrix = half_band_width.get_size_of_desparsed_stiffness_matrix();
  std::vector<int>&accumulative_half_band_width_vector = half_band_width.get_accumulative_half_band_width_vector();

  BoundaryCondition boundary_condition;
  boundary_condition.InitializeBoundaryCondition(&initialization);

//  boundary_condition.PrintBoundaryConditionNodes(essential_bc_nodes);

  HeaterElements heater_elements;
  heater_elements.InitializeHeaterElements(&initialization);
  heater_elements.set_elements_as_heater(&initialization);
//  heater_elements.PrintHeaterElements();
  int num_of_elements_as_heater=heater_elements.get_num_of_elements_as_heater();
  std::vector<int>&elements_as_heater=heater_elements.get_elements_as_heater();

  RadiationElements radiation_elements;
  radiation_elements.InitializeRadiationElements(&initialization);
  radiation_elements.set_elements_with_radiation(&initialization);
//  radiation_elements.PrintRadiationElements();
  int num_of_elements_with_radiation=radiation_elements.get_num_of_elements_with_radiation();
  std::vector<int> &elements_with_radiation=radiation_elements.get_elements_with_radiation(); 

  MaterialParameters material_parameters;
  material_parameters.set_densities();
  material_parameters.set_material_id_of_elements(&initialization);
//  material_parameters.PrintMaterialParameters();
  std::vector<int>& material_id_of_elements = material_parameters.get_material_id_of_elements();
  std::vector<double>& densities = material_parameters.get_densities();

  GlobalVectorsAndMatrices global_vectors_and_matrices;
  global_vectors_and_matrices.InitializeGlobalVectorsAndMatrices(num_of_nodes, accumulative_half_band_width_vector);
  std::vector<double>& stiffness_matrix = global_vectors_and_matrices.get_stiffness_matrix();
  std::vector<double>& mass_matrix = global_vectors_and_matrices.get_mass_matrix();
  std::vector<double>& radiation_tangential_matrix = global_vectors_and_matrices.get_radiation_tangential_matrix();
  std::vector<double>& body_heat_flux_tangential_matrix = global_vectors_and_matrices.get_body_heat_flux_tangential();
  std::vector<double>& heat_load = global_vectors_and_matrices.get_heat_load();
  std::vector<double>& radiation_load = global_vectors_and_matrices.get_radiation_load();
  std::vector<double>& current_temperature_field = global_vectors_and_matrices.get_current_temperature_field();
  std::vector<double>& right_hand_side_function = global_vectors_and_matrices.get_right_hand_side_function();
  std::vector<double>& jacobian_matrix_global = global_vectors_and_matrices.get_jacobian_matrix_global();
  std::vector<double>& solution_increments_trial = global_vectors_and_matrices.get_solution_increments_trial();
  std::vector<double>& initial_temperature_field = global_vectors_and_matrices.get_initial_temperature_field();
  std::vector<double>& solution_of_last_iteration = global_vectors_and_matrices.get_solution_of_last_iteration();

  TemperatureFieldInitial temperature_field_initial;
  temperature_field_initial.set_initial_temperature_field(essential_bc_nodes, initial_temperature_field, &initialization, 
  equation_numbers_of_nodes);
//  temperature_field_initial.PrintInitialTemperatureField(initial_temperature_field);

  ElementalStiffnessMatrix elemental_stiffness_matrix;
  elemental_stiffness_matrix.InitializeElementalStiffnessMatrix();
  ElementalMassMatrix elemental_mass_matrix;
  elemental_mass_matrix.InitializeElementalMassMatrix();

  ElementalBodyHeatFluxTangentialMatrix elemental_body_heat_flux_tangential_matrix;
  elemental_body_heat_flux_tangential_matrix.InitializeElementalBodyHeatFluxTangentialMatrix();
  ElementalRadiationTangentialMatrixAndRadiationLoad elemental_radiation_tangential_matrix_and_radiation_load;
  elemental_radiation_tangential_matrix_and_radiation_load.InitializeElementalRadiationTangentialMatrixAndRadiationLoad();
  Assemble assemble;
  Solver solver;
  OutputResults output_results;

  for(int time_step=0; current_time<=total_simulation_time; time_step++){
    if(time_step>=maximum_time_steps){
      printf("maximum time steps has been reached. simulation aborted\n");
      exit(-1);
    }  
 
    for(int i=0; i<current_temperature_field.size(); i++)
      current_temperature_field[i]=initial_temperature_field[i];  // set initial values to dLastitersolu[]
    while(1){
      global_vectors_and_matrices.ZeroVectorAndMatrix();

      for(int element_number=0;element_number<num_of_elements;element_number++){
        elemental_stiffness_matrix.set_coordinates_in_this_element(element_number, nodes_in_elements, x_coordinates, y_coordinates);
        elemental_stiffness_matrix.set_element_stiffness_matrix(element_number, nodes_in_elements, material_id_of_elements, 
          current_temperature_field, &temperature_dependent_variables);
        elemental_stiffness_matrix.MapElementalToGlobalStiffness(stiffness_matrix, accumulative_half_band_width_vector, 
          equation_numbers_in_elements, element_number);
        std::vector<std::vector<double> >&element_stiffness_matrix = elemental_stiffness_matrix.get_element_stiffness_matrix();

        elemental_mass_matrix.set_coordinates_in_this_element(element_number, nodes_in_elements, x_coordinates, y_coordinates);
        elemental_mass_matrix.set_element_mass_matrix(element_number, nodes_in_elements, material_id_of_elements, current_temperature_field, 
          &temperature_dependent_variables, densities, time_increment);
        elemental_mass_matrix.MapElementalToGlobalMass(mass_matrix, accumulative_half_band_width_vector,equation_numbers_in_elements, 
          element_number);

        boundary_condition.FixTemperature(element_number, element_stiffness_matrix, equation_numbers_in_elements, heat_load);
      }


      for(int heater_element_number=0; heater_element_number<num_of_elements_as_heater; heater_element_number++){
        int element_number=elements_as_heater[heater_element_number];
        elemental_body_heat_flux_tangential_matrix.set_coordinates_in_this_element(element_number, nodes_in_elements, x_coordinates, 
          y_coordinates);
        elemental_body_heat_flux_tangential_matrix.set_element_body_heat_flux_tangential_matrix(element_number, heater_element_number, 
          nodes_in_elements, current_temperature_field, &temperature_dependent_variables, &initialization);
        elemental_body_heat_flux_tangential_matrix.MapElementalToGlobalBodyHeatFluxTangentialMatrix(body_heat_flux_tangential_matrix, 
          accumulative_half_band_width_vector, equation_numbers_in_elements, element_number);

        heater_elements.set_coordinates_in_this_element(element_number, nodes_in_elements, x_coordinates, y_coordinates);
        heater_elements.HeatSupply(element_number, heater_element_number, heat_load, nodes_in_elements, 
        equation_numbers_in_elements,current_temperature_field, &temperature_dependent_variables,&initialization);
      }
 
//    elemental_body_heat_flux_tangential_matrix.PrintBodyHeatFluxTangentialMatrix(body_heat_flux_tangential_matrix);

      for(int radiation_element_number=0;radiation_element_number<num_of_elements_with_radiation;radiation_element_number++){
        int element_number=elements_with_radiation[radiation_element_number];
        elemental_radiation_tangential_matrix_and_radiation_load.set_element_radiation_tangential_matrix_and_radiation_load(element_number, 
          radiation_element_number, nodes_in_elements, current_temperature_field, &temperature_dependent_variables, x_coordinates, 
          ambient_temperature);
        elemental_radiation_tangential_matrix_and_radiation_load.MapElementalToGlobalRadiationTangentialMatrixAndRadiationLoad
          (radiation_tangential_matrix, accumulative_half_band_width_vector, radiation_load, equation_numbers_in_elements, element_number);
      }

//      elemental_radiation_tangential_matrix_and_radiation_load.PrintRadiationTangentialMatrixAndRadiationLoad(radiation_tangential_matrix,
//        radiation_load);

      assemble.AssembleGlobalJacobian(&global_vectors_and_matrices);
      assemble.AssembleGlobalYfunction(equation_numbers_of_nodes, &global_vectors_and_matrices);
//      assemble.PrintGlobalJacobian(&global_vectors_and_matrices);
//      assemble.PrintGlobalYfunction(&global_vectors_and_matrices);


//     printf("%.6f %.6f\n",solver.NormOfVector(solution_of_last_iteration),solver.NormOfVector(right_hand_side_function));

      if(solver.NormOfVector(solution_of_last_iteration)<Constants::kNormTolerance_ && 
      solver.NormOfVector(right_hand_side_function)<Constants::kYFunctionTolerance_){// convergence must be satisfied first, then consider temperature increment size.

        for(int j=0; j<num_of_nodes; j++){
          check_temperature_change_size_satisfiable=true;
          if(fabs(current_temperature_field[j]-initial_temperature_field[j])>maximum_temperature_change_per_time_increment){
            time_increment /= 4; 
            if(time_increment<(minimum_time_increment)){
              printf("time increment size is too small. simulation aborted!\n");
              exit(-1);
            }
            printf("reduce time increment size1\n");
            num_of_iterations_with_unchanged_time_increment=0;
            iteration_number=0; //zero back the iteration num counting;
            for(int i=0; i<num_of_nodes; i++)
              current_temperature_field[i]=initial_temperature_field[i];  // set initial values to current_temperature_field[]
            check_temperature_change_size_satisfiable=false;

            break; // break from the for loop
          }
        }//for
        if(check_temperature_change_size_satisfiable==false) continue; //temperature change is too much, reduce it and try again

        if(time_to_turn_off_heaters!=0.0){ // if heaters will be turned off during the simulation
          if(is_heaters_turned_off==false && (current_time+time_increment)>time_to_turn_off_heaters 
              && time_to_turn_off_heaters-current_time>=minimum_time_increment){
            printf("cut time increment to reach the time point to turn off heaters\n");
            time_increment = (time_to_turn_off_heaters-current_time);
            num_of_iterations_with_unchanged_time_increment=0;
            iteration_number=0; //zero back the iteration num counting;
            for(int i=0; i<num_of_nodes; i++)
              current_temperature_field[i]=initial_temperature_field[i];  // set initial values to current_temperature_field[]
            continue; //continue the while loop
          }
          if((current_time+time_increment)==time_to_turn_off_heaters){
            for(int k=0; k<(*(initialization.get_currents_in_heater())).get_current_in_heater().size(); k++)
              (*(initialization.get_currents_in_heater())).get_current_in_heater()[k]=0.0;
            is_heaters_turned_off==true;
          }
        }

        ++num_of_iterations_with_unchanged_time_increment;
        printf("number of iterations with unchanged time increment size is %d\n",num_of_iterations_with_unchanged_time_increment);

        current_time+=time_increment;  //update the current time

        if(num_of_iterations_with_unchanged_time_increment==2){ // check if time increment size is stable for recent two steps. then increase time increment size
          time_increment *= 2; 
          num_of_iterations_with_unchanged_time_increment=0;
          iteration_number=0; //zero back the iteration num counting;
          printf("time increment size increased\n");
        }
        printf("number of iteration to converge is %d\n",iteration_number);
        break;  // break from the while loop
      }//if

      ++iteration_number;

      if(solver.LinearEquationsSolver(&global_vectors_and_matrices)==1){
        time_increment /= 4; 
        if(time_increment<(minimum_time_increment)){
          printf("time increment size is too small. simulation aborted!\n");
          exit(-1);
        }
        printf("reduce time increment size2\n");
        num_of_iterations_with_unchanged_time_increment=0;
        iteration_number=0; //zero back the iteration num counting;
        for(int i=0; i<num_of_nodes; i++)
          current_temperature_field[i]=initial_temperature_field[i];  // set initial values to current_temperature_field[]

        continue;
      }

      if(iteration_number>Constants::kMaxNewtonIteration_){
        time_increment /= 4; 
        if(time_increment<(minimum_time_increment)){
          printf("time increment size is too small. simulation aborted!\n");
          exit(-1);
        }
        printf("reduce time increment size3\n");
        num_of_iterations_with_unchanged_time_increment=0;
        iteration_number=0; //zero back the iteration num counting;
        for(int i=0; i<num_of_nodes; i++)
          current_temperature_field[i]=initial_temperature_field[i];  // set initial values to current_temperature_field[]
        continue;
      }

      for(int j=0; j<num_of_nodes; j++){
        int equation_count=equation_numbers_of_nodes[j];
        if(equation_count>=0)
          current_temperature_field[j] += solution_of_last_iteration[equation_count];
      }

    }//while

    for(int i=0; i<num_of_nodes; i++){
      initial_temperature_field[i]=current_temperature_field[i];
    }

    temperature_norm_last = temperature_norm_current;
    temperature_norm_current = solver.NormOfVector(initial_temperature_field);

    if(fabs(temperature_norm_current - temperature_norm_last) < Constants::kNormTolerance_ ){//when temperature is in steady state, terminate program.
      output_results.OutputVtkFile(time_step, current_time, &initialization, &generate_mesh, initial_temperature_field);
      output_results.OutputCopperSurfaceTemperature(time_step, current_time, &initialization, &generate_mesh, initial_temperature_field);
      printf("the %dth time integration completed\n\n", time_step+1);     
      break;
    }
    if((time_step+1)%output_time_step_interval==0 || time_step==maximum_time_steps-1 || current_time>total_simulation_time 
       || current_time==time_to_turn_off_heaters){ 
      output_results.OutputVtkFile(time_step, current_time, &initialization, &generate_mesh, initial_temperature_field);
      output_results.OutputCopperSurfaceTemperature(time_step, current_time, &initialization, &generate_mesh, initial_temperature_field);     
    }
    printf("the %dth time integration completed\n\n", time_step+1);
  }

  FILE* current_densities;
  current_densities=fopen("current_densities.txt","w");
//  fprintf(current_densities,"units are A/m^2\n");
  fprintf(current_densities,"units are A/cm^2\n");
//  double heater_cross_section_area=temperature_dependent_variables.get_heater_crosssection_area_mm_square()*1.0e-3*1.0e-3; //mm^2 -> m^2
  double heater_cross_section_area=temperature_dependent_variables.get_heater_crosssection_area_mm_square()*1.0e-1*1.0e-1; //mm^2 -> cm^2
  for(int i=0;i<Constants::kNumOfHeaters_;i++){
    double current=(*(initialization.get_currents_in_heater())).get_current_in_heater()[i]*1.0e-3; //mA -> A
    fprintf(current_densities,"%e\n",current/heater_cross_section_area);
  }
  fclose(current_densities);

  printf("Analysis completed successfully!\n");
  printf("several (model temperature field).vtk files, (copper surface temperature).txt files and a (current_density).txt file have been generated\n\n");
  return 0;
}
