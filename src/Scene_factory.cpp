#include "Scene.h"

#include "Resource_manager.h"
#include "Drawable_object.h"
#include "Light.h"
#include "Camera.h"
#include "Shader_program.h"
#include "Model.h"
#include "Texture.h"
#include "Material.h"

// Transforms
#include "../include/common/TR_common.h"

Scene *login_scene()
{
    Scene *scene = new Scene(0);
    auto &res = Resource_manager::instance();

    auto shader = res.load_shader("../resources/shaders/light.vert", "../resources/shaders/light.frag");

    auto login_model = res.load_model("../resources/models/KOZ0382_text.obj");
    auto login = std::make_shared<Drawable_object>();
    login->set_model(login_model);
    login->add_shader(shader);
    auto plane_texture = res.load_texture("../resources/objects/diglett/textures/rock.jpeg");
    login->set_texture(plane_texture);
    login->add_static_transform(new Rotation(vec3(0.0f, glm::radians(180.0f), 0.0f)));
    scene->add_entity(login);

    auto light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Directional);
    light->attach_observer(shader);
    scene->add_entity(light);

    auto camera = std::make_shared<Camera>();
    camera->attach_observer(shader);
    scene->add_entity(camera);

    return scene;
}

Scene *whack_the_mole()
{
    Scene *scene = new Scene(2);

    auto shader_program = std::make_shared<Shader_program>();

    shader_program->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/light.vert");
    shader_program->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/light.frag");

    auto shader_program_skybox = std::make_shared<Shader_program>();

    shader_program_skybox->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/skycube.vert");
    shader_program_skybox->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/skycube.frag");

    auto &res = Resource_manager::instance();

    auto teren_model = res.load_model("../resources/objects/teren/teren.obj");
    auto teren = std::make_shared<Drawable_object>();
    teren->set_model(teren_model);
    teren->add_shader(shader_program);
    auto teren_texture = res.load_texture("../resources/objects/teren/grass.png");
    teren->set_texture(teren_texture);
    scene->add_entity(teren);

    auto cube_model = res.load_model("../resources/objects/sky/skybox.obj");

    auto obj_2 = std::make_shared<Drawable_object>();
    obj_2->set_model(cube_model);
    obj_2->add_shader(shader_program_skybox);

    auto texture_skybox = std::make_shared<Texture>();

    const std::vector<std::string> paths = {"../resources/textures/cubemap/posx.jpg", "../resources/textures/cubemap/negx.jpg", "../resources/textures/cubemap/posy.jpg", "../resources/textures/cubemap/negy.jpg", "../resources/textures/cubemap/posz.jpg", "../resources/textures/cubemap/negz.jpg"};

    // texture_skybox->load_from_files(paths);
    texture_skybox = res.load_cubemap(paths);
    obj_2->set_texture(texture_skybox);

    obj_2->add_static_transform(new Scale(vec3(120.0f)));
    obj_2->add_static_transform(new Transfer(vec3(0.0f, 10.0f, 0.0f)));
    scene->add_entity(obj_2);

    auto camera = std::make_shared<Camera>();
    camera->attach_observer(shader_program);
    camera->attach_observer(shader_program_skybox);

    scene->add_entity(camera);

    auto light = std::make_shared<Light>(vec3(0.0f, 1.0f, 0.0f), LightType::Directional);
    light->attach_observer(shader_program);
    scene->add_entity(light);

    auto diglett_model = res.load_model("../resources/objects/diglett/diglett.fbx");

    auto diglett = std::make_shared<Drawable_object>();
    diglett->set_model(diglett_model);
    diglett->add_shader(shader_program);

    // diglett->add_local_transform(new Transfer(vec3(0.1f, 0.0f, 0.0f)));
    diglett->add_local_transform(new Timer_transform({{0.4, new Rotation(glm::vec3(0.0f, radians(0.0f), 0.0f), glm::vec3(0.0f, radians(360.0f), 0.0f))}, {0, new Transfer(glm::vec3(0.0f, 0.0f, 0.04f))}}));

    diglett->set_id(2);
    diglett->set_time_to_live(3);
    auto diglett_texture = res.load_texture("../resources/objects/diglett/textures/rock.jpeg");
    // diglett->set_texture(diglett_texture);
    diglett->set_color(vec3(0.9, 0.5, 0.3));

    diglett->add_local_transform(new Parametric_transform({
                                                              vec3(0.0f, 0.0f, 0.0f),
                                                              vec3(0.0f, 1.0f, 0.0f),
                                                              vec3(0.0f, 0.0f, 0.0f),
                                                          },
                                                          1.0f));

    diglett->get_material()->set_specular(vec3(0));
    scene->set_spawn_entity(diglett);
    scene->add_entity(diglett);

    scene->set_random_spawn(1.0f, vec3(-5.0f, 0.0f, -5.0f), vec3(5.0f, 0.0f, 5.0f));

    return scene;
}

Scene *sphere_scene()
{
    Scene *scene = new Scene(1);

    auto &res = Resource_manager::instance();

    auto sphere = res.load_model("../resources/models/Sphere.obj");

    vector<shared_ptr<Shader_program>> shader_programs;

    shader_programs.push_back(std::make_shared<Shader_program>());
    shader_programs.back()->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/basic.vert");
    shader_programs.back()->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/basic.frag");

    shader_programs.push_back(std::make_shared<Shader_program>());
    shader_programs.back()->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/light.vert");
    shader_programs.back()->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/lambert.frag");

    shader_programs.push_back(std::make_shared<Shader_program>());
    shader_programs.back()->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/light.vert");
    shader_programs.back()->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/phong.frag");

    shader_programs.push_back(std::make_shared<Shader_program>());
    shader_programs.back()->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/light.vert");
    shader_programs.back()->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/blinn.frag");

    shared_ptr<Drawable_object> obj;

    vector<shared_ptr<Drawable_object>> objects;

    for (int i = 0; i < 4; i++)
    {
        obj = std::make_shared<Drawable_object>(sphere, shader_programs[i], vec3(0.8f, 0.2f, 0.4f));

        objects.push_back(obj);
    }

    objects[0]->add_static_transform(new Transfer(vec3(0.0f, -2.0f, 0.0f)));
    objects[1]->add_static_transform(new Transfer(vec3(0.0f, 2.0f, 0.0f)));
    objects[2]->add_static_transform(new Transfer(vec3(-2.0f, 0.0f, 0.0f)));
    objects[3]->add_static_transform(new Transfer(vec3(2.0f, 0.0f, 0.0f)));

    auto camera = std::make_shared<Camera>();

    for (auto &o : objects)
    {
        scene->add_entity(o);
    }
    for (auto &shader_program : shader_programs)
    {
        camera->attach_observer(shader_program);
    }
    scene->add_entity(camera);

    auto light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f));
    for (auto &shader_program : shader_programs)
    {
        light->attach_observer(shader_program);
    }
    scene->add_entity(light);

    return scene;
}

Scene *forest_scene()
{
    Scene *scene = new Scene(3);

    auto &res = Resource_manager::instance();

    shared_ptr<Model> model = std::make_shared<Model>();
    auto tree_model = res.load_model("../resources/models/tree.obj");

    auto bush_model = res.load_model("../resources/objects/bush/Bush.obj");
    auto sphere = res.load_model("../resources/models/Sphere.obj");

    auto bush_texture = res.load_texture("../resources/objects/bush/Grass.png");

    shared_ptr<Drawable_object> obj = std::make_shared<Drawable_object>();
    shared_ptr<Shader_program> shader_program = std::make_shared<Shader_program>();

    shader_program->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/light.vert");
    shader_program->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/light_blinn.frag");

    auto shader_program_2 = std::make_shared<Shader_program>();
    shader_program_2->load_shader_from_file(GL_VERTEX_SHADER, "../resources/shaders/basic.vert");
    shader_program_2->load_shader_from_file(GL_FRAGMENT_SHADER, "../resources/shaders/basic.frag");

    // Skybox
    auto cube_model = res.load_model("../resources/objects/sky/skybox.obj");

    auto shader_program_skybox = res.load_shader("../resources/shaders/skycube.vert", "../resources/shaders/skycube.frag");

    auto obj_2 = std::make_shared<Drawable_object>();
    obj_2->set_model(cube_model);
    obj_2->add_shader(shader_program_skybox);

    auto texture_skybox = res.load_cubemap({"../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg"});

    obj_2->set_texture(texture_skybox);

    scene->set_skybox(obj_2);
    // Plane

    auto plane_model = res.load_model("../resources/objects/teren/teren.obj");
    obj = std::make_shared<Drawable_object>(plane_model, shader_program);
    auto plane_texture = res.load_texture("../resources/objects/teren/grass.png");
    obj->set_texture(plane_texture);
    obj->add_static_transform(new Scale(vec3(0.25)));
    obj->add_static_transform(new Transfer(vec3(7.0f, -0.02f, 9.0f)));
    scene->add_entity(obj);

    for (int i = 0; i < 100; i++)
    {

        obj = std::make_shared<Drawable_object>(tree_model, shader_program);

        obj->add_static_transform(new Transfer(vec3(rand() % 100 / (float)5, 0.0f, 1 + rand() % 100 / (float)6)));

        float random_scale = (float)(rand() % 20) / (float)100 + 0.1f;
        obj->add_static_transform(new Scale(glm::vec3(random_scale, random_scale, random_scale)));

        scene->add_entity(obj);
    }

    for (int i = 0; i < 3600; i++)
    {

        int y_pos = floor(i / (float)60);
        int x_pos = i - 60 * y_pos;

        obj = std::make_shared<Drawable_object>(bush_model, shader_program, vec3(0.1f, 0.6f, 0.1f));
        obj->set_texture(bush_texture);

        obj->add_static_transform(new Transfer(vec3((float)x_pos / 3 + (rand() % 3 + 1) / 10.0f, -0.02f, (float)y_pos / 3 + (rand() % 3 + 1) / 10.0f)));

        float random_scale = (float)(rand() % 20) / (float)50 + 0.70f;
        obj->add_static_transform(new Scale(glm::vec3(random_scale, random_scale, random_scale)));

        scene->add_entity(obj);
    }

    shared_ptr<Light> light;
    for (int i = 0; i < 5; i++)
    {
        int y_pos = floor(i / (float)3);
        int x_pos = (i - 3 * y_pos);

        light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Point, 2.0f, 0.05f);
        light->set_color(vec3(1.0f, 1.0f, 1.0f));
        light->set_model(sphere);
        light->add_shader(shader_program_2);

        light->add_static_transform(new Scale(glm::vec3(0.02f, 0.02f, 0.02f)));

        // light->add_static_transform(new Transfer(glm::vec3(2.0f, 0.1f, 3.5f)));
        light->add_static_transform(new Transfer(glm::vec3(3 + x_pos * 5, 0.2f, 3 + y_pos * 10)));

        light->add_local_transform(new Timer_transform({{0.3, new Rotation(glm::vec3(-radians(0.1f), -radians(40.0f), -radians(0.1f)), glm::vec3(radians(0.1f), radians(40.0f), radians(0.1f)))}, {0, new Transfer(glm::vec3(0.0f, 0.0f, 0.02f))}}));

        light->attach_observer(shader_program);
        scene->add_entity(light);
    }

    // Shrek
    auto shrek_model = res.load_model("../resources/objects/shrek/shrek.obj");
    auto shrek_texture = res.load_texture("../resources/objects/shrek/shrek.png");

    auto shrek = std::make_shared<Drawable_object>(shrek_model, shader_program);
    shrek->add_static_transform(new Scale(vec3(0.5f)));
    shrek->add_static_transform(new Transfer(vec3(6.0f, 0.0f, 6.0f)));
    shrek->set_id(1); // Setting stencil ID to 1

    shrek->set_texture(shrek_texture);
    scene->add_entity(shrek);

    shrek->add_local_transform(new Timer_transform({{0.7, new Rotation(glm::vec3(0.0f, -radians(10.0f), 0.0f), glm::vec3(0.0f, radians(20.0f), 0.0f))}, {0, new Transfer(glm::vec3(0.0f, 0.0f, 0.04f))}}));

    // Fiona
    auto fiona_model = res.load_model("../resources/objects/shrek/fiona.obj");
    auto fiona_texture = res.load_texture("../resources/objects/shrek/fiona.png");

    auto fiona = std::make_shared<Drawable_object>(fiona_model, shader_program);
    fiona->add_static_transform(new Scale(vec3(0.5f)));
    fiona->add_static_transform(new Transfer(vec3(2.0f, 0.0f, 2.0f)));
    fiona->add_local_transform(new Bezier_transform({vec3(1.0f, 0.0f, 1.0f),
                                                     vec3(2.0f, 0.0f, 2.0f),
                                                     vec3(2.0f, 0.0f, 0.0f),
                                                     vec3(1.0f, 0.0f, 1.0f),
                                                     vec3(0.0f, 0.0f, 2.0f),
                                                     vec3(0.0f, 0.0f, 0.0f),
                                                     vec3(1.0f, 0.0f, 1.0f)},
                                                    0.3f));

    fiona->set_id(2); // Setting stencil ID to 2
    fiona->set_texture(fiona_texture);
    fiona->get_material()->set_ambient(vec3(1.0f));
    fiona->get_material()->set_specular(vec3(0.0f));
    fiona->get_material()->set_diffuse(vec3(1.0f));
    fiona->get_material()->set_shininess(32.0f);

    scene->add_entity(fiona);

    std::shared_ptr<Light> light2 = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Directional, 0.001f, 1.0f);
    light2->add_static_transform(new Transfer(vec3(0.0f, 5.0f, 0.0f)));
    light2->attach_observer(shader_program);
    scene->add_entity(light2);

    std::shared_ptr<Camera> camera = std::make_shared<Camera>();
    camera->attach_observer(shader_program);
    camera->attach_observer(shader_program_2);
    camera->attach_observer(shader_program_skybox);
    scene->add_entity(camera);

    obj_2->add_parent(camera->get_transform_manager(), TransformParentType::Skybox);
    scene->set_skybox(obj_2);

    auto flash_light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Reflector, 1.0f, 0.7f);
    flash_light->set_model(sphere);
    flash_light->set_color(vec3(0.5f));
    flash_light->add_shader(shader_program_2);

    flash_light->add_parent(camera->get_transform_manager());
    flash_light->add_static_transform(new Transfer(vec3(0.2f, -0.2f, -0.2f)));

    flash_light->add_static_transform(new Scale(vec3(0.2f)));

    flash_light->attach_observer(shader_program);
    scene->add_entity(flash_light);

    auto ambient_light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Ambient, 1.0f, 0.6f);
    ambient_light->attach_observer(shader_program);
    scene->add_entity(ambient_light);

    auto s = std::make_shared<Drawable_object>(sphere, shader_program);

    scene->set_spawn_entity(s);

    return scene;
}

Scene *solar_system_scene()
{
    Scene *scene = new Scene(4);

    auto sphere = std::make_shared<Model>();
    sphere->load_from_file("../resources/models/planet.obj");

    auto &res = Resource_manager::instance();

    auto shader_program = res.load_shader("../resources/shaders/light.vert", "../resources/shaders/light.frag");

    // Textures
    auto earth_texture = res.load_texture("../resources/textures/solar_system/2k_earth.jpg");
    earth_texture->load_from_file("../resources/textures/solar_system/2k_earth.jpg");

    auto moon_texture = res.load_texture("../resources/textures/solar_system/2k_moon.jpg");
    auto sun_texture = res.load_texture("../resources/textures/solar_system/2k_sun.jpg");

    auto mercury_texture = res.load_texture("../resources/textures/solar_system/2k_mercury.jpg");

    auto mars_texture = res.load_texture("../resources/textures/solar_system/2k_mars.jpg");

    auto jupiter_texture = res.load_texture("../resources/textures/solar_system/2k_jupiter.jpg");

    // Skybox
    auto cube_model = res.load_model("../resources/objects/sky/skybox.obj");

    auto shader_program_skybox = res.load_shader("../resources/shaders/skycube.vert", "../resources/shaders/skycube.frag");

    auto skybox = std::make_shared<Drawable_object>();
    skybox->set_model(cube_model);
    skybox->add_shader(shader_program_skybox);

    auto texture_skybox = res.load_cubemap({"../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg",
                                            "../resources/textures/solar_system/starfield1.jpg"});

    //  texture_skybox->load_from_files(paths);
    skybox->set_texture(texture_skybox);

    // Planets
    // Sun
    auto sun = std::make_shared<Light>();
    sun->attach_observer(shader_program);
    sun->set_model(sphere);
    sun->add_shader(shader_program);
    sun->set_texture(sun_texture);
    sun->add_static_transform(new Scale(vec3(10.0f)));
    sun->add_local_transform(new Rotation(vec3(0.0f, radians(0.1f), 0.0f)));
    sun->set_material(std::make_shared<Material>(vec3(1.0f), vec3(1.0f), vec3(0.0f), 1.0f));

    scene->add_entity(sun);

    // Mercury
    auto mercury = std::make_shared<Drawable_object>();
    mercury->set_model(sphere);
    mercury->add_shader(shader_program);
    mercury->set_texture(mercury_texture);

    mercury->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.8f), 0.0f))); // orbit
    mercury->add_local_transform(new Rotation(glm::vec3(0.0f, radians(2.0f), 0.0f)));  // spin

    mercury->add_static_transform(new Transfer(vec3(14.0f, 0.0f, 0.0f))); // distance from sun
    mercury->add_static_transform(new Scale(vec3(0.4f)));                 // size
    mercury->get_material()->set_specular(vec3(0.1f));

    // Mars
    auto mars = std::make_shared<Drawable_object>();
    mars->set_model(sphere);
    mars->add_shader(shader_program);
    mars->set_texture(mars_texture);

    mars->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.3f), 0.0f))); // orbit
    mars->add_local_transform(new Rotation(glm::vec3(0.0f, radians(3.0f), 0.0f)));  // spin

    mars->add_static_transform(new Transfer(vec3(17.0f, 0.0f, 0.0f))); // distance from sun
    mars->add_static_transform(new Scale(vec3(0.7f, 0.7f, 0.7f)));     // size
    mars->get_material()->set_specular(vec3(0.1f));

    // Earth
    auto earth = std::make_shared<Drawable_object>();
    earth->set_model(sphere);
    earth->add_shader(shader_program);
    earth->set_texture(earth_texture);

    earth->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.5f), 0.0f))); // orbit
    earth->add_local_transform(new Rotation(glm::vec3(0.0f, radians(0.5f), 0.0f)));  // spin

    earth->add_static_transform(new Transfer(vec3(22.0f, 0.0f, 0.0f))); // distance from sun
    earth->add_static_transform(new Scale(vec3(1.0f)));                 // size
    earth->get_material()->set_specular(vec3(0.1f));

    // Moon
    auto moon = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Point, 0.4f, 0.9f);
    moon->set_model(sphere);
    moon->add_shader(shader_program);

    moon->add_parent(earth->get_transform_manager());
    moon->add_static_transform(new Transfer(vec3(2.0f, 0.0f, 0.0f)));
    moon->set_texture(moon_texture);
    moon->add_global_transform(new Rotation(glm::vec3(0.0f, radians(-10.3f), 0.0f))); // moon orbit
    moon->add_local_transform(new Rotation(glm::vec3(0.0f, radians(1.0f), 0.0f)));    // moon spin
    moon->add_static_transform(new Scale(vec3(0.3f, 0.3f, 0.3f)));
    moon->get_material()->set_specular(vec3(0.1f));

    // Jupiter
    auto jupiter = std::make_shared<Drawable_object>();
    jupiter->set_model(sphere);
    jupiter->add_shader(shader_program);
    jupiter->set_texture(jupiter_texture);

    jupiter->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.1f), 0.0f))); // orbit
    jupiter->add_local_transform(new Rotation(glm::vec3(0.0f, radians(0.08), 0.0f)));  // spin

    jupiter->add_static_transform(new Transfer(vec3(35.0f, 0.0f, 0.0f))); // distance from sun
    jupiter->add_static_transform(new Scale(vec3(2.0f)));                 // size
    jupiter->get_material()->set_specular(vec3(0.1f));

    // multiple jupiter moons
    for (int i = 0; i < 5; i++)
    {
        auto jupiter_moon = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Point, 0.4f, 0.9f);
        jupiter_moon->set_model(sphere);
        jupiter_moon->add_shader(shader_program);

        jupiter_moon->add_parent(jupiter->get_transform_manager());
        jupiter_moon->add_static_transform(new Transfer(vec3(2.1 + rand() % 10 / (float)10, 0.0f, 0.0f)));
        jupiter_moon->set_texture(moon_texture);
        jupiter_moon->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.1 + rand() % 10 / (float)10), 0.0f))); // jupiter_moon orbit
        jupiter_moon->add_local_transform(new Rotation(glm::vec3(0.0f, radians(0.1 + rand() % 10 / (float)10), 0.0f)));  // jupiter_moon spin
        jupiter_moon->add_static_transform(new Scale(vec3(0.1 + rand() % 10 / (float)100)));
        jupiter_moon->get_material()->set_specular(vec3(0.1f));
        scene->add_entity(jupiter_moon);
    }

    auto login_model = res.load_model("../resources/models/KOZ0382_text.obj");
    auto login = std::make_shared<Drawable_object>();
    login->set_model(login_model);
    login->add_shader(shader_program);
    auto plane_texture = res.load_texture("../resources/objects/diglett/textures/rock.jpeg");
    login->set_texture(plane_texture);
    login->add_static_transform(new Rotation(vec3(0.0f, glm::radians(180.0f), 0.0f)));
    login->add_static_transform(new Scale(vec3(0.5f)));
    login->add_static_transform(new Transfer(vec3(-12.0f, 0.0f, 0.0f)));
    login->add_local_transform(new Rotation(vec3(0.0f, radians(1.0f), 0.0f)));
    login->add_global_transform(new Rotation(glm::vec3(0.0f, radians(0.2f), 0.0f)));

    auto login_2 = std::make_shared<Drawable_object>();
    login_2->set_model(login_model);
    login_2->add_shader(shader_program);
    login_2->set_texture(plane_texture);
    login_2->add_static_transform(new Rotation(vec3(0.0f, glm::radians(180.0f), 0.0f)));
    login_2->add_static_transform(new Scale(vec3(0.5f)));
    login_2->add_parent(earth->get_transform_manager());
    login_2->add_static_transform(new Transfer(vec3(-3.0f, 0.0f, 0.0f)));
    login_2->add_local_transform(new Rotation(vec3(0.0f, radians(10.0f), 0.0f)));
    login_2->add_global_transform(new Rotation(glm::vec3(0.0f, radians(2.0f), 0.0f)));

    scene->add_entity(login);
    scene->add_entity(login_2);

    scene->add_entity(earth);
    scene->add_entity(mercury);
    scene->add_entity(mars);
    scene->add_entity(jupiter);
    scene->add_entity(moon);

    // Additional lights
    auto ambient_light = std::make_shared<Light>(vec3(1.0f, 1.0f, 1.0f), LightType::Ambient, 0.92f, 1.0f);
    ambient_light->attach_observer(shader_program);
    scene->add_entity(ambient_light);

    // Camera
    auto camera = std::make_shared<Camera>();
    camera->attach_observer(shader_program);
    camera->attach_observer(shader_program_skybox);
    camera->set_position(vec3(0.0f, 8.0f, 20.0f));
    scene->add_entity(camera);

    skybox->add_parent(camera->get_transform_manager(), TransformParentType::Skybox);
    scene->set_skybox(skybox);

    return scene;
}
