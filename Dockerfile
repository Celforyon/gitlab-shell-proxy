FROM alpine

LABEL maintainer="Alexis Pereda <alexis@pereda.fr>"
LABEL version="1.2"
LABEL description="Gitlab shell proxy"

RUN apk add --no-cache bash inotify-tools

COPY ./entrypoint /usr/local/bin/entrypoint
COPY ./monitor /usr/local/bin/monitor
COPY ./gitlab-shell-proxy /default/gitlab-shell-proxy

ENV USER          "git"
ENV USER_UID      "1000"
ENV GITLAB_HOST   "git.example.org.docker"
ENV GITLAB_SHELL  "/opt/gitlab/embedded/service/gitlab-shell/bin/gitlab-shell"

ENV IAUTHDIR      "/in"
ENV OAUTHDIR      "/out"

ENV COMMAND       "~/bin/gitlab-shell-proxy"

ENTRYPOINT ["/usr/local/bin/entrypoint", "/usr/local/bin/monitor"]
