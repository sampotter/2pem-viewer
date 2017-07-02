#include "options.hpp"

#include <iostream>
#include <stdexcept>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <glclient/lua.hpp>

options
options::from_cli_args(int argc, char ** argv)
{
    using namespace boost::program_options;

    options_description desc {"Testing testing"};

    desc.add_options()
        ("help,h", "Display help")
        ("config_file",
         "The path to a configuration file to use to specify these options.")
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
        ("slm_res_x",
         value<std::size_t>()->default_value(1920),
         "The width in pixels of the spatial light modulator (SLM) display.")
        ("slm_res_y",
         value<std::size_t>()->default_value(1080),
         "The height in pixels of the SLM display.")
        ("slm_dim_x",
         value<double>()->default_value(15.36),
         "The width in mm of the SLM display.")
        ("slm_dim_y",
         value<double>()->default_value(8.64),
         "The height in mm of the SLM display.")
        ("gs_iter_count",
         value<std::size_t>()->default_value(10),
         "The default number of Gerchberg-Saxton iterations used to compute "
         "the SLM phase mask.")
#if USE_OSC
        ("osc_port9",
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

    if (varmap.count("config_file")) {
        auto const path = varmap["config_file"].as<std::string>();
        return options::from_config_file(path);
    }

    slm_parameters slm_params;
    slm_params.resolution.width = varmap["slm_res_x"].as<std::size_t>();
    slm_params.resolution.height = varmap["slm_res_y"].as<std::size_t>();
    slm_params.dimensions.width = varmap["slm_dim_x"].as<double>();
    slm_params.dimensions.height = varmap["slm_dim_y"].as<double>();

    lens_parameters lens_params;
    lens_params.focal_length = varmap["focal_length"].as<double>();
    lens_params.wavelength = varmap["wavelength"].as<double>();
    lens_params.origin.x = varmap["origin_x"].as<std::size_t>();
    lens_params.origin.y = varmap["origin_y"].as<std::size_t>();

    return options {
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

options
options::from_config_file(std::string const & path)
{
    boost::filesystem::path fs_path {path};
    if (!boost::filesystem::exists(fs_path)) {
        throw std::runtime_error(path + " does not exist");
    }
    

    lua::state lua;
    lua.do_file(fs_path.string());
    

    lua.get_global("hostname");
    auto const hostname = lua.to_string(-1);
    lua.pop(1);

    lua.get_global("port");
    auto const port = lua.to_string(-1);
    lua.pop(1);

    lua.get_global("image_parameters");

    lua.get_field(-1, "width");
    auto const image_width = lua.to_integer(-1);
    lua.pop(1);

    lua.get_field(-1, "height");
    auto const image_height = lua.to_integer(-1);
    lua.pop(2);

    lua.get_global("gerchberg_saxton_iterations");
    auto const gs_iters = lua.to_integer(-1);
    lua.pop(1);

#if USE_OSC
    lua.get_global("osc_port");
    auto const osc_port = lua.to_integer(-1);
    lua.pop(1);
#endif // USE_OSC

    lua.get_global("num_mean_frames");
    auto const num_mean_frames = lua.to_integer(-1);
    lua.pop(1);
    

    slm_parameters slm_params;

    lua.get_global("slm_parameters");
    
    lua.get_field(-1, "resolution");
    
    lua.get_field(-1, "width");
    slm_params.resolution.width = lua.to_integer(-1);
    lua.pop(1);

    lua.get_field(-1, "height");
    slm_params.resolution.height = lua.to_integer(-1);
    lua.pop(2);

    lua.get_field(-1, "dimensions");

    lua.get_field(-1, "width");
    slm_params.dimensions.width = lua.to_number(-1);
    lua.pop(1);

    lua.get_field(-1, "height");
    slm_params.dimensions.height = lua.to_number(-1);
    lua.pop(3);
    

    lens_parameters lens_params;

    lua.get_global("lens_parameters");

    lua.get_field(-1, "focal_length");
    lens_params.focal_length = lua.to_number(-1);
    lua.pop(1);

    lua.get_field(-1, "wavelength");
    lens_params.wavelength = lua.to_number(-1);
    lua.pop(1);

    lua.get_field(-1, "origin");

    lua.get_field(-1, "x");
    lens_params.origin.x = lua.to_integer(-1);
    lua.pop(1);

    lua.get_field(-1, "y");
    lens_params.origin.y = lua.to_integer(-1);
    lua.pop(3);

    
    return options {
        hostname,
        port,
        static_cast<std::size_t>(image_width),
        static_cast<std::size_t>(image_height),
        static_cast<std::size_t>(gs_iters),
#if USE_OSC
        osc_port,
#endif // USE_OSC
        static_cast<std::size_t>(num_mean_frames),
        slm_params,
        lens_params
    };
}

std::string
options::get_hostname() const
{
    return hostname_;
}

std::string
options::get_port() const
{
    return port_;
}

std::size_t
options::get_img_width() const
{
    return img_width_;
}

std::size_t
options::get_img_height() const
{
    return img_height_;
}

std::size_t
options::get_gs_iter_count() const
{
    return gs_iter_count_;
}

#if USE_OSC
std::size_t
options::get_osc_port() const
{
    return osc_port_;
}
#endif // USE_OSC

std::size_t
options::get_num_mean_frames() const
{
    return num_mean_frames_;
}

options::options(std::string const & hostname
               , std::string const & port
               , std::size_t img_width
               , std::size_t img_height
               , std::size_t gs_iter_count
#if USE_OSC
               , std::size_t osc_port
#endif // USE_OSC
               , std::size_t num_mean_frames
               , slm_parameters slm_params
               , lens_parameters lens_params):
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
options::get_slm_parameters() const
{
    return slm_params_;
}

lens_parameters const &
options::get_lens_parameters() const
{
    return lens_params_;
}

// Local Variables:
// indent-tabs-mode: nil
// End:
