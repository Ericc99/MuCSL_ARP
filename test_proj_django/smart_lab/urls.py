from django.urls import path, include

from rest_framework.routers import DefaultRouter

from .views import OperatorViewSet, ChemicalViewSet

router = DefaultRouter()
router.register("chemicals", ChemicalViewSet, basename="chemicals")

urlpatterns = [
    path('', include(router.urls)),
]