#include "client_options.hpp"

#include <iostream>

#include <boost/program_options.hpp>

client_options
client_options::from_cli_args(int argc, char ** argv)
{
    using namespace boost::program_options;

    options_description desc {"Testing testing"};

    desc.add_options()
        ("help,h", "Display help")
        ("hostname",
         value<std::string>()->default_value("localhost"),
         "The server hostname")
        ("port",
         value<std::string>()->default_value("8888"),
         "The server port")
        ("image_width",
         value<std::size_t>()->default_value(512),
         "The width of the image in pixels")
        ("image_height",
         value<std::size_t>()->default_value(512),
         "The height of the image in pixels")
        ("slm_width",
         value<std::size_t>()->default_value(1024),
         "The width of the spatial light modulator (SLM) display.")
        ("slm_height",
         value<std::size_t>()->default_value(768),
         "The height of the SLM display.")
        ("gs_iter_count",
         value<std::size_t>()->default_value(10),
         "The default number of Gerchberg-Saxton iterations used to compute "
         "the SLM phase mask.")
#if USE_OSC
        ("osc_port",
         value<std::size_t>()->default_value(9000),
         "The port that will receive OSC messages")
#endif // USE_OSC
        ("num_mean_frames",
         value<std::size_t>()->default_value(10),
         "The number of frames to use to compute an average frame.")
        ("focal_length",
         value<double>()->default_value(150.0),
         "The focal length of the fresnel lens.")
        ("wavelength",
         value<double>()->default_value(1040.0),
         "The wavelength of the laser.")
        ("origin_x",
         value<std::size_t>()->default_value(0),
         "The x-coordinate of the SLM pattern.")
        ("origin_y",
         value<std::size_t>()->default_value(0),
         "The y-coordinate of the SLM pattern.")
      ;

    variables_map varmap;
    store(parse_command_line(argc, argv, desc), varmap);
    notify(varmap);

    if (varmap.count("help")) {
        std::cout << desc << std::endl;
        std::exit(EXIT_FAILURE);
    }

    slm_parameters slm_params;
    slm_params.resolution.width = varmap["slm_dim_x"].as<std::size_t>();
    slm_params.resolution.height = varmap["slm_dim_y"].as<std::size_t>();
    slm_params.dimensions.width = varmap["slm_res_x"].as<double>();
    slm_params.dimensions.height = varmap["slm_res_y"].as<double>();

    lens_parameters lens_params;
    lens_params.focal_length = varmap["focal_length"].as<double>();
    lens_params.wavelength = varmap["wavelength"].as<double>();
    lens_params.origin.x = varmap["origin_x"].as<std::size_t>();
    lens_params.origin.y = varmap["origin_y"].as<std::size_t>();

    return client_options {
        varmap["hostname"].as<std::string>()
      , varmap["port"].as<std::string>()
      , varmap["image_width"].as<std::size_t>()
      , varmap["image_height"].as<std::size_t>()
      , varmap["gs_iter_count"].as<std::size_t>()
#if USE_OSC
      , varmap["osc_port"].as<std::size_t>()
#endif // USE_OSC
      , varmap["num_mean_frames"].as<std::size_t>()
      , slm_params
      , lens_params
    };
}

std::string
client_options::get_hostname() const
{
    return hostname_;
}

std::string
client_options::get_port() const
{
    return port_;
}

std::size_t
client_options::get_img_width() const
{
    return img_width_;
}

std::size_t
client_options::get_img_height() const
{
    return img_height_;
}

std::size_t
client_options::get_gs_iter_count() const
{
    return gs_iter_count_;
}

#if USE_OSC
std::size_t
client_options::get_osc_port() const
{
    return osc_port_;
}
#endif // USE_OSC

std::size_t
client_options::get_num_mean_frames() const
{
    return num_mean_frames_;
}

client_options::client_options(std::string const & hostname
                             , std::string const & port
                             , std::size_t img_width
                             , std::size_t img_height
                             , std::size_t gs_iter_count
#if USE_OSC
                             , std::size_t osc_port
#endif // USE_OSC
                             , std::size_t num_mean_frames
                             , slm_parameters slm_params
                             , lens_parameters lens_params
                               ):
    hostname_ {hostname}
  , port_ {port}
  , img_width_ {img_width}
  , img_height_ {img_height}
  , gs_iter_count_ {gs_iter_count}
#if USE_OSC
  , osc_port_ {osc_port}
#endif // USE_OSC
  , num_mean_frames_ {num_mean_frames}
  , slm_params_ {slm_params}
  , lens_params_ {lens_params}
{}

slm_parameters const &
client_options::get_slm_parameters() const
{
    return slm_params_;
}

lens_parameters const &
client_options::get_lens_parameters() const
{
    return lens_params_;
}

// Local Variables:
// indent-tabs-mode: nil
// End:
