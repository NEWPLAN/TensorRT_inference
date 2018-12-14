#include <inference_sys_director.h>
#include <inference_sys_builder.h>
#include <np_inference_sys_builder.h>
#include <np_inference_sys_director.h>
#include <chrono>
#include <thread>

int main(int argc, char const *argv[])
{
    log_sys logs(argv[0]);
    InferenceSysDirector *director = new NPInferenceSysDirector(new NPInferenceSysBuilder());
    InferenceSys *infer_sys = director->create_inferenceSys();
    infer_sys->get_engine()->display();
    infer_sys->get_parser()->display();
    infer_sys->get_dataCollector()->display();

    std::string a("100");
    infer_sys->get_parser()->setup(a);
    infer_sys->get_engine()->setup(argc, argv);
    infer_sys->get_dataCollector()->setup(a);

    while (1)
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        LOG(INFO) << "main thread loops....";
    }
    return 0;
}
