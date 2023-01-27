#! /bin/bash

show_help() {
    echo "USAGE:"
    echo "  $0 [flag]"
    echo "OPTIONS:"
    echo " [none]   initialize and run program"
    echo "  -h      help: show this menu"
    echo "  -i      initialize: start a new docker container using dotFinder docker image" 
    echo "  -r      run: launch dotfinder docker container and link output to local dir"
    echo "  -c      clean up: stop dotfinder docker container and remove docker image from local system"
    echo "  -b      build: force local rebuild of dotfinder docker image (use if pulled image does not work)"
    echo "  -e      build: force local rebuild of base Linux environment docker image"
}

if [[ $# -gt 1 ]]; then
    echo "Error; please select just one flag at a time (or none)"
    exit 0
fi

OPTIND=1
init=false
run=false
clean=false 
build=false
build_base=false
while getopts "h?ircbde" opt; do
    if $init OR $run OR $clean OR $build OR $build_base; then
        echo "Error; please select just one flag at a time (or none)"
        exit 0
    fi
    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    i)  
        init=true
        ;;
    r)  
        run=true
        ;;
    c)  
        clean=true
        ;;
    b)  
        build=true
        ;;
    e)
        build_base=true
        ;;
    esac
done

shift $((OPTIND-1))
[ "${1:-}" = "--" ] && shift

if $init; then
    echo "Initializing dotFinder docker container named 'dotfinder' (please wait...)"
    docker run --rm -itd -v "${PWD}":/mnt --name "dotfinder" shfiorenza/dotfinder bash
elif $run; then
    echo "Running dotFinder program in docker container. Data files will be output to local directory"
    echo "(Interactive launcher only; quick-launch syntax currently not supported w/ Docker)"
    echo ""
    docker exec -it dotfinder dot_finder.exe
elif $clean; then 
    echo "Stopping dotFinder container and removing docker image from local system."
    docker container stop dotfinder
    docker image rm shfiorenza/dotfinder
elif $build; then
    echo "Building dotfinder docker image"
    docker build --no-cache -t shfiorenza/dotfinder:latest docker
elif $build_base; then
    echo "Building dotfinder base environment docker image"
    docker build --no-cache -f docker/Dockerfile_environment -t shfiorenza/dotfinder_base:latest docker
else
    echo "Initializing dotFinder docker container named 'dotfinder' (please wait...)"
    docker run --rm -itd -v "${PWD}":/mnt --name "cylaks" shfiorenza/cylaks bash
    echo "Running dotFinder program in docker container. Data files will be output to local directory"
    echo "(Interactive launcher only; quick-launch syntax currently not supported w/ Docker)"
    echo ""
    docker exec -it dotfinder dot_finder.exe
fi
