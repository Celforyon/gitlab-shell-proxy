# Gitlab Shell Proxy

If you are using Gitlab inside a container, you can use this container to help you setting up a proxy for Gitlab shell (allowing `git fetch`, `git push`, ... commands).

## Setup

Steps:
- have a running instance of Gitlab inside a docker
- create an user (typically `git`) in the **host**
- generate a SSH key for this user
- add it in the Gitlab shell authorized_keys
- run the gitlab-shell-proxy container

### Create user
```bash
useradd -md /home/git git
```

### Generate SSH key
```bash
su git -c 'ssh-keygen -t rsa -b 4096'
```

### Add it in the Gitlab shell authorized_keys
```bash
sed -i -e '1i # gitlab-shell-proxy' -e "1i $(cat ~git/.ssh/id_rsa.pub)" gitlab/authorized_keys
```

### Run the gitlab-shell-proxy container
See Container section for docker-compose.yml examples and available configuration

## Container

### example of docker-compose.yml

```yml
version: '2'
services:
  gitlab-shell-proxy:
    container_name: gitlab-shell-proxy
    image: celforyon/gitlab-shell-proxy
    restart: always
    volumes:
    - /.../gitlab/.ssh:/in:ro
    - /home/git/.ssh:/out
    - /home/git/bin:/proxy
    environment:
    - USER=git
    - USER_UID=1000
    - GITLAB_HOST=git.example.org.docker
    depends_on:
    - gitlab
```

### Environment variables

| Name | Value | Default value |
| --- | --- | --- |
| USER | **host** user name | git |
| USER_UID | **host** user UID | 1000 |
| GITLAB_HOST | IP or hostname of the Gitlab container | git.example.org.docker |
| GITLAB_SHELL | the gitlab-shell binary in the Gitlab container | /opt/gitlab/embedded/service/gitlab-shell/bin/gitlab-shell |
| IAUTHDIR | the path of input authorized_keys parent directory | /in |
| OAUTHDIR | the path of output authorized_keys parent directory | /out |
| COMMAND | the **host** location of gitlab-shell-proxy | ~/bin/gitlab-shell-proxy |
