hostname = "localhost"
port = 8888

image_parameters = {
   width = 512,
   height = 512
}

gerchberg_saxton_iterations = 10

osc_port = 9000

num_mean_frames = 5

slm_parameters = {
   resolution = {
	  width = 1920,
	  height = 1080
   },
   dimensions = {
	  width = 15.36,
	  height = 8.64
   }
}
	  
lens_parameters = {
   focal_length = 150,
   wavelength = 1040,
   origin = {
	  x = slm_parameters.resolution.width/2,
	  y = sim_parameters.resolution.height/2,
   }
}
